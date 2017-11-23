#include <stdlib.h>
#include <inttypes.h>

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/x509.h>

#include <jansson.h>

#include "DisC_types.h"
#include "DisC_util.h"
#include "DisC_errors.h"
#include "DisC_rest.h"

const char *hostname = "discordapp.com:https";
const char *host = "discordapp.com";
const char *apiBase = "/api/v6";


char *internal_GetHTTPResponseCode(DisC_session_t *session, char *str);

char *internal_GenerateHeaders(DisC_session_t *session, char *url, char *httpAction, const char *contentType, unsigned long contentLength);

unsigned char *internal_ReadData(DisC_session_t *session, unsigned long *dataLen);

short internal_WriteData(DisC_session_t *session, unsigned char *data, unsigned long dataLen);

//======================================================================================

char *internal_GetHTTPResponseCode(DisC_session_t *session, char *str)
{
  //make a temp string to verify the send
  char *tempReturn = NULL;
  DisC_asprintf(&tempReturn, "%s", str);
  char *httpStatus = strtok(tempReturn, "\r\n");
  char *code = httpStatus + strlen("HTTP/1.1 ");
  strtok(code, " ");
  //finish getting status

  char *returnStr = DisC_strmkdup(code);
  free(tempReturn);

  return returnStr;
}

char *internal_GenerateHeaders(DisC_session_t *session, char *url, char *httpAction, const char *contentType, unsigned long contentLength)
{
  char *final = NULL;
  char *base = "%s %s HTTP/1.1\r\nHost: %s\r\nConnection: keep-alive\r\nAuthorization: %s%s\r\nUser-Agent: DisC v%d.%d.%d: %s\r\n%s\r\n%s";

  if(strcmp(httpAction, "GET") != 0)
  {
    //add a mime type and content length:
    DisC_asprintf(&final, base, httpAction, url, host, session->clientType ? "Bot " : "", session->token, DISC_VER_MAJOR, DISC_VER_MINOR, DISC_VER_REVISION, session->internalName, "Content-Type: %s\r\nContent-Length: %lu\r\n", "%s");
    //printf("%s\n", final);
    DisC_asprintf(&final, final, contentType, contentLength, "%s");
    //printf("%s\n", final);
  }
  else
  {
    //otherwise just set the %s to nothing
    DisC_asprintf(&final, base, httpAction, url, host, session->clientType ? "Bot " : "", session->token, DISC_VER_MAJOR, DISC_VER_MINOR, DISC_VER_REVISION, session->internalName, "", "");
  }

  return final;
}

unsigned char *internal_ReadData(DisC_session_t *session, unsigned long *dataLen)
{
  char *buffer = NULL;
  unsigned long byteCount = 0;
  unsigned long byteChunkMax = 4;
  unsigned long status;
  int firstRun = 0;
  int isChunked = 2;
  int chunkedCanExit = 0;

  buffer = malloc(byteChunkMax);
  if(buffer == NULL)
  {
    exit(1);
  }
  status = byteChunkMax;
  do {
    status = (unsigned long)BIO_read(session->DONOTSET_rest_bio, buffer + byteCount, status);
    if(status == -1 && firstRun == 0)
    {
      return NULL;
    }
    else if(firstRun == 0)
    {
      firstRun++;
    }

    byteCount += status;

    buffer = realloc(buffer, byteCount + byteChunkMax);
    //printf("%d\n", status);
    if(strstr(buffer, "Server: ") != NULL && isChunked == 2)
    {
      if(strstr(buffer, "Transfer-Encoding: chunked")!= NULL)
      {
        isChunked = 1;
      }
      else
      {
        isChunked = 0;
      }
    }

    //printf("buffer: %s\n", buffer);

    if(isChunked)
    {
      if(strstr(buffer, "\r\n0\r\n"))
      {
        chunkedCanExit = 1;
      }
      /*
      if(strstr(buffer, "\r\n\r\n") != NULL)
      {
        char *tempLen = NULL;
        DisC_asprintf(&tempLen, "%s", buffer);
        char *lengthChar = strstr(tempLen, "\r\n\r\n");
        lengthChar += strlen("\r\n\r\n");
        char *temp = strstr(lengthChar, "\r\n");
        if(temp != NULL)
        {
          *temp = 0x00;
        }

        while()

        //long length = strtol(lengthChar, NULL, 16);

        //printf("hex: %s, then length: %lu\n", lengthChar, length);


        free(tempLen);
      }
      */
    }

  } while(status > 0 && chunkedCanExit == 0);

  //buffer[byteCount] = '\0';//not always going to be text data. Set in rest functions

  buffer = realloc(buffer, byteCount);


  return buffer;
}

short internal_WriteData(DisC_session_t *session, unsigned char *data, unsigned long dataLen)
{
  while(BIO_write(session->DONOTSET_rest_bio, data, dataLen) <= 0)
  {
    short result = DisC_REST_ReconnectSession(session);//reconnect if necessary, then just return if it couldnt reconnect
    if(result)
    {
      DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_ERROR, "Could not retry or reconnect after attempting to send REST data");
      DisC_AddError(session, DISC_ERROR_NONE);
      return DISC_ERROR_NONE;
    }
  }
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Wrote data", data);
  DisC_AddError(session, DISC_ERROR_NONE);
  return DISC_ERROR_NONE;
}

//public functions

short DisC_REST_Init()
{
  SSL_load_error_strings();
  SSL_library_init();
  ERR_load_BIO_strings();
  OpenSSL_add_all_algorithms();

  return DISC_ERROR_NONE;
}

short DisC_REST_ReconnectSession(DisC_session_t *session)
{
  unsigned long reconnNum = 0;
  while(DisC_REST_InitSession(session) != DISC_ERROR_NONE)
  {
    if(DISC_MAX_RECONNECTS != -1)
    {
      reconnNum++;
    }
    DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Reconnecting to %s..", hostname);
    sleep(DISC_RECONNECT_DELAY);
    if(reconnNum == DISC_MAX_RECONNECTS)
    {
      DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Could not reconnect");
      DisC_AddError(session, DISC_ERROR_CONNECTION);
      return DISC_ERROR_CONNECTION;
    }
  }
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Reconnected");
  DisC_AddError(session, DISC_ERROR_NONE);
  return DISC_ERROR_NONE;
  //fix state stuff if needed
}

short DisC_REST_InitSession(DisC_session_t *session)
{
  //set the session ssl context to TLS 1.2
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Connecting to %s...", hostname);
  session->DONOTSET_rest_ctx = SSL_CTX_new(TLSv1_2_client_method());


  session->DONOTSET_rest_bio = BIO_new_ssl_connect(session->DONOTSET_rest_ctx);
  BIO_set_conn_hostname(session->DONOTSET_rest_bio, hostname);

  //connect
  if(BIO_do_connect(session->DONOTSET_rest_bio) <= 0)
  {
    DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_ERROR, "Could not connect");
    DisC_REST_ReconnectSession(session);
    DisC_AddError(session, DISC_ERROR_CONNECTION);
    return DISC_ERROR_CONNECTION;
  }
  else
  {
    DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Connected");
    DisC_AddError(session, DISC_ERROR_NONE);
    return DISC_ERROR_NONE;
  }
}

short DisC_REST_DestroySession(DisC_session_t *session)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Disconnecting from %s...", hostname);
  BIO_free_all(session->DONOTSET_rest_bio);
  SSL_CTX_free(session->DONOTSET_rest_ctx);

  DisC_AddError(session, DISC_ERROR_NONE);
  return DISC_ERROR_NONE;
}

//=============================================================================================================================================

short DisC_REST_GetChannel(DisC_session_t *session, DisC_snowflake_t channelId, DisC_channel_t *channel)
{
  short returnval;
  char *base = NULL;
  char *url = NULL;
  char *readData = NULL;
  unsigned long readDataLen = 0;

  DisC_asprintf(&url, "%s/channels/%s", apiBase, channelId);
  base = internal_GenerateHeaders(session, url, "GET", NULL, 0);

  //printf("%s\n", base);

  internal_WriteData(session, base, strlen(base));
  readData = internal_ReadData(session, &readDataLen);

  //json stuff
  //printf("%s\n", readData);



  json_t *retRoot = NULL;

  char *responseJson = strstr(readData, "\r\n\r\n");
  responseJson += strlen("\r\n\r\n");
  responseJson = strstr(responseJson, "\r\n");
  responseJson += strlen("\r\n");
  char *endResponseJson = strstr(responseJson, "\r\n");
  *endResponseJson = 0x00;
  //printf("1[%s]\n\n", responseJson);


  retRoot = json_loads(responseJson, 0, NULL);

  channel->id = DisC_strmkdup(json_string_value(json_object_get(retRoot, "id")));
  channel->type = json_integer_value(json_object_get(retRoot, "type"));
  channel->guild_id = DisC_strmkdup(json_string_value(json_object_get(retRoot, "guild_id")));
  channel->position = json_integer_value(json_object_get(retRoot, "position"));
  //TODO OVERWRITE ARRAY OBJECTS
  channel->name = DisC_strmkdup(json_string_value(json_object_get(retRoot, "name")));
  channel->topic = DisC_strmkdup(json_string_value(json_object_get(retRoot, "topic")));
  channel->isNsfw = json_integer_value(json_object_get(retRoot, "nsfw"));
  channel->last_message_id = DisC_strmkdup(json_string_value(json_object_get(retRoot, "last_message_id")));
  channel->bitrate = json_integer_value(json_object_get(retRoot, "bitrate"));
  channel->user_limit = json_integer_value(json_object_get(retRoot, "user_limit"));
  //TODO recipients
  channel->icon = DisC_strmkdup(json_string_value(json_object_get(retRoot, "icon")));
  channel->owner_id = DisC_strmkdup(json_string_value(json_object_get(retRoot, "owner_id")));
  channel->application_id = DisC_strmkdup(json_string_value(json_object_get(retRoot, "application_id")));
  channel->parent_id = DisC_strmkdup(json_string_value(json_object_get(retRoot, "parent_id")));
  channel->last_pin_timestamp = DisC_strmkdup(json_string_value(json_object_get(retRoot, "last_pin_timestamp")));

  char *code = internal_GetHTTPResponseCode(session, readData);


  //============================================

  if(strcmp(code, "200") == 0)
  {
    DisC_AddError(session, DISC_ERROR_NONE);
    returnval = DISC_ERROR_NONE;
  }
  else if(strcmp(code, "401") == 0)
  {
    DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_WARNING, "Insufficient permissions for action");
    DisC_AddError(session, DISC_ERROR_PERMISSIONS);
    returnval = DISC_ERROR_PERMISSIONS;
  }
  else{
    DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_WARNING, "HTTP error: %s", code);
    DisC_AddError(session, DISC_ERROR_GENERIC);
    returnval = DISC_ERROR_GENERIC;
  }

  json_decref(retRoot);
  free(code);
  free(readData);
  free(base);
  free(url);

  return returnval;
}

short DisC_REST_CreateMessage(DisC_session_t *session, DisC_snowflake_t channelId, char *content, DisC_snowflake_t nonce, DisC_BOOL_t tts, unsigned char *fileData, unsigned long fileDataLen, DisC_embed_t *embed)//get channel id and add parameters
{
  short returnval;
  char *base = NULL;
  char *url = NULL;
  char *final = NULL;

  char *readData = NULL;
  unsigned long readDataLen = 0;

  json_t *root = json_object();

  json_object_set_new(root, "content", json_string(content));
  if(nonce != NULL)
  {
    json_object_set_new(root, "noone", json_string(nonce));
  }
  json_object_set_new(root, "tts", json_boolean(tts));
  if(fileData != NULL)
  {
    json_object_set_new(root, "file", json_integer(fileDataLen));
  }
  if(embed != NULL)
  {
    json_object_set_new(root, "embed", json_string("test"));//TODO add embed parsing from the array of embeds possibly
  }
  if(fileData != NULL)
  {
    json_object_set_new(root, "payload_json", json_string("test"));
  }

  //===================================================================

  //DisC_asprintf(&url, "%s/channels/%"PRIu64"/messages", apiBase, channelId);
  DisC_asprintf(&url, "%s/channels/%s/messages", apiBase, channelId);
  if(fileData != NULL)
  {
    base = internal_GenerateHeaders(session, url, "POST", "multipart/form-data", strlen(json_dumps(root, 0)));
  }
  else
  {
    base = internal_GenerateHeaders(session, url, "POST", "application/json", strlen(json_dumps(root, 0)));
  }

  DisC_asprintf(&final, base, json_dumps(root, 0));

  internal_WriteData(session, final, strlen(final));
  readData = internal_ReadData(session, &readDataLen);



  char *code = internal_GetHTTPResponseCode(session, readData);


  //============================================

  if(strcmp(code, "200") == 0)
  {
    DisC_AddError(session, DISC_ERROR_NONE);
    returnval = DISC_ERROR_NONE;
  }
  else if(strcmp(code, "401") == 0)
  {
    DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_WARNING, "Insufficient permissions for action");
    DisC_AddError(session, DISC_ERROR_PERMISSIONS);
    returnval = DISC_ERROR_PERMISSIONS;
  }
  else{
    DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_WARNING, "HTTP error: %s", code);
    DisC_AddError(session, DISC_ERROR_GENERIC);
    returnval = DISC_ERROR_GENERIC;
  }

  //free everything==================================================
  json_decref(root);
  free(code);
  free(url);
  free(final);
  free(readData);

  return returnval;
}
