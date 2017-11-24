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
  //temporary fix for chunked response still sending data that code later mistakes for the end of headers
  if(*str == '0')
  {
    if(strstr(str, "\r\n\r\n") == &str[1])
    {
      str[1] = "eror";
    }
  }
  char *tempReturn = NULL;
  DisC_asprintf(&tempReturn, "%s", strstr(str, "HTTP/1.1 "));
  char *httpStatus = strtok(tempReturn, "\r\n");
  char *code = httpStatus + strlen("HTTP/1.1 ");
  strtok(code, " ");
  //printf("str: %s\ncode: %s\nand: %s\ncode: %s\n", str, httpStatus, tempReturn, code);
  //finish getting status

  char *returnStr = DisC_strmkdup(code);
  free(tempReturn);

  return returnStr;
}

char *internal_GenerateHeaders(DisC_session_t *session, char *url, char *httpAction, const char *contentType, unsigned long contentLength)
{
  char *final = NULL;
  char *base = "%s %s HTTP/1.1\r\nHost: %s\r\nConnection: keep-alive\r\nAuthorization: %s%s\r\nUser-Agent: DisC v%d.%d.%d: %s\r\n%s\r\n%s";

  if(strcmp(httpAction, "GET") == 0 || strcmp(httpAction, "DELETE") == 0)
  {
    //otherwise just set the %s to nothing
    DisC_asprintf(&final, base, httpAction, url, host, session->clientType ? "Bot " : "", session->token, DISC_VER_MAJOR, DISC_VER_MINOR, DISC_VER_REVISION, session->internalName, "", "");
  }
  else
  {
    //add a mime type and content length:
    DisC_asprintf(&final, base, httpAction, url, host, session->clientType ? "Bot " : "", session->token, DISC_VER_MAJOR, DISC_VER_MINOR, DISC_VER_REVISION, session->internalName, "Content-Type: %s\r\nContent-Length: %lu\r\n", "%s");
    //printf("%s\n", final);
    DisC_asprintf(&final, final, contentType, contentLength, "%s");
    //printf("%s\n", final);
  }

  return final;
}

unsigned char *internal_ReadData(DisC_session_t *session, unsigned long *dataLen)
{
  char *buffer = NULL;
  unsigned long byteCount = 0;
  unsigned long byteChunkMax = 1024;
  unsigned long contentLength = 0;
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
      //printf("Found Type\n");
      if(strstr(buffer, "Transfer-Encoding: chunked")!= NULL)
      {
        //printf("found chunked\n");
        isChunked = 1;
      }
      else if(strstr(buffer, "Content-Length"))
      {
        //printf("found contentlength\n");
        isChunked = 0;

        char *tempStr = NULL;
        DisC_asprintf(&tempStr, "%s", buffer);

        char *tempPtr = strstr(tempStr, "Content-Length");
        tempPtr = strtok(tempPtr, "\r\n");

        tempPtr += strlen("Content-Length: ");


        contentLength = (unsigned long)strtol(tempPtr, NULL, 10);
        //printf("thing: %lu\n", contentLength);
        free(tempStr);
      }
    }

    //printf("buffer: %s\nstatus: %lu", buffer, status);

    if(isChunked == 1)
    {
      //printf("Detecting chunk...\n");
      if(strstr(buffer, "\r\n0\r\n"))
      {
        BIO_read(session->DONOTSET_rest_bio, NULL, strlen("0\r\n\r\n"));
        chunkedCanExit = 1;
      }

    }
    else if(contentLength > 0)
    {
      if(strstr(buffer, "\r\n\r\n") != NULL)
      {
        //printf("found end of headers\n");
        char *marked = strstr(buffer, "\r\n\r\n");
        unsigned long i;
        for(i = 0; i < byteCount; i++)
        {
          if(&buffer[i] == marked)
          {

            i += strlen("\r\n\r\n");

            if(byteCount - i == contentLength)
            {
              chunkedCanExit = 1;//reusing a variable
              //printf("can exit\n");
            }
            else
            {
              //printf("cant: %lu\n", byteCount - i);
            }


          }
        }
      }
    }

  } while(status > 0 && chunkedCanExit == 0);



  //buffer[byteCount] = '\0';//not always going to be text data. Set in rest functions

  buffer = realloc(buffer, byteCount);

  *dataLen = byteCount;
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
  //printf("writing:\n\n\n%s\n\n\n", data);
  //DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Wrote data", data);
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
    DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Reconnecting to %s..", host);
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
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Connecting to %s...", host);
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
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Disconnecting from %s...", host);
  BIO_free_all(session->DONOTSET_rest_bio);
  SSL_CTX_free(session->DONOTSET_rest_ctx);

  DisC_AddError(session, DISC_ERROR_NONE);
  return DISC_ERROR_NONE;
}

//=============================================================================================================================================

short DisC_REST_GetChannel(DisC_session_t *session, DisC_snowflake_t channelId, DisC_channel_t *channel)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Getting channel");
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

  char *finalData = malloc(readDataLen + 1);//turn it into a string
  if(finalData == NULL)
  {
    exit(1);
  }
  memmove(finalData, readData, readDataLen);
  //finalData[readDataLen + 1] = 0x00;//terminate string
  finalData[readDataLen] = 0x00;//terminate string

  json_t *retRoot = NULL;

  char *code = internal_GetHTTPResponseCode(session, finalData);

  //============================================

  if(strcmp(code, "200") == 0)
  {
    char *responseJson = strstr(finalData, "\r\n\r\n");
    responseJson += strlen("\r\n\r\n");
    responseJson = strstr(responseJson, "\r\n");
    responseJson += strlen("\r\n");
    char *endResponseJson = strstr(responseJson, "\r\n");
    *endResponseJson = 0x00;
    //printf("1[%s]\n\n", responseJson);

    if(responseJson != NULL)
    {
      printf("Parsing JSON\n");
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

      //json_decref(retRoot);//It doesnt like to free stuff???

      DisC_AddError(session, DISC_ERROR_NONE);
      returnval = DISC_ERROR_NONE;
    }
    else
    {
      DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_ERROR, "Failed to parse JSON");
      DisC_AddError(session, DISC_ERROR_MEMORY);
      returnval = DISC_ERROR_MEMORY;
    }

  }
  else if(strcmp(code, "401") == 0)
  {
    DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_WARNING, "Insufficient permissions for action");
    DisC_AddError(session, DISC_ERROR_PERMISSIONS);
    returnval = DISC_ERROR_PERMISSIONS;
  }
  else{
    DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_WARNING, "HTTP error: %s: %s", code, strstr(finalData, "\r\n\r\n"));
    DisC_AddError(session, DISC_ERROR_GENERIC);
    returnval = DISC_ERROR_GENERIC;
  }


  //free(code);
  //free(finalData);
  //free(readData);
  //free(base);
  //free(url);

  return returnval;
}

short DisC_REST_ModifyChannel(DisC_session_t *session, DisC_snowflake_t channelId, char *name, int position, char *topic, DisC_BOOL_t nsfw, int bitrate, int userLimit, DisC_overwrite_t *overwrites, DisC_snowflake_t parentId)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Modifying channel");
  short returnval;
  char *base = NULL;
  char *url = NULL;
  char *final = NULL;
  char *readData = NULL;
  unsigned long readDataLen = 0;


  json_t *root = json_object();

  if(name != NULL)
    json_object_set_new(root, "name", json_string(name));

  if(position >= 0)
    json_object_set_new(root, "position", json_integer(position));

  if(topic != NULL)
    json_object_set_new(root, "topic", json_string(topic));

  if(nsfw >= 0)
  json_object_set_new(root, "nsfw", json_boolean(nsfw));

  if(bitrate >= 0)
    json_object_set_new(root, "bitrate", json_integer(bitrate));

  if(userLimit >= 0)
    json_object_set_new(root, "user_limit", json_integer(userLimit));

  if(overwrites != NULL)
    json_object_set_new(root, "permission_overwrites", json_string("test"));

  if(parentId != NULL)
    json_object_set_new(root, "parent_id", json_string(parentId));

  //===================================================================

  DisC_asprintf(&url, "%s/channels/%s", apiBase, channelId);
  base = internal_GenerateHeaders(session, url, "PUT", "application/json", strlen(json_dumps(root, 0)));


  DisC_asprintf(&final, base, json_dumps(root, 0));

  //printf("[[%s]]\n", final);

  internal_WriteData(session, final, strlen(final));
  readData = internal_ReadData(session, &readDataLen);

  char *finalData = malloc(readDataLen + 1);//turn it into a string
  if(finalData == NULL)
  {
    exit(1);
  }
  memmove(finalData, readData, readDataLen);
  //finalData[readDataLen + 1] = 0x00;//terminate string
  finalData[readDataLen] = 0x00;//terminate string


  char *code = internal_GetHTTPResponseCode(session, finalData);


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
    DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_WARNING, "HTTP error: %s: %s", code, strstr(finalData, "\r\n\r\n"));
    DisC_AddError(session, DISC_ERROR_GENERIC);
    returnval = DISC_ERROR_GENERIC;
  }

  //free everything==================================================
  json_decref(root);
  free(code);
  free(url);
  free(finalData);
  free(final);
  free(base);
  free(readData);

  return returnval;
}

short DisC_REST_DeleteChannel(DisC_session_t *session, DisC_snowflake_t channelId)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Deleting channel");
  short returnval;
  char *base = NULL;
  char *url = NULL;
  char *readData = NULL;
  unsigned long readDataLen = 0;

  DisC_asprintf(&url, "%s/channels/%s", apiBase, channelId);
  base = internal_GenerateHeaders(session, url, "DELETE", NULL, 0);



  //printf("[[%s]]\n", base);

  internal_WriteData(session, base, strlen(base));
  readData = internal_ReadData(session, &readDataLen);

  char *finalData = malloc(readDataLen + 1);//turn it into a string
  if(finalData == NULL)
  {
    exit(1);
  }
  memmove(finalData, readData, readDataLen);
  //finalData[readDataLen + 1] = 0x00;//terminate string
  finalData[readDataLen] = 0x00;//terminate string

  char *code = internal_GetHTTPResponseCode(session, finalData);


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
    DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_WARNING, "HTTP error: %s: %s", code, strstr(finalData, "\r\n\r\n"));
    DisC_AddError(session, DISC_ERROR_GENERIC);
    returnval = DISC_ERROR_GENERIC;
  }

  //free everything==================================================
  free(code);
  free(url);
  free(finalData);
  free(base);
  free(readData);

  return returnval;
}

short DisC_REST_GetChannelMessages(DisC_session_t *session, DisC_snowflake_t channelId, DisC_snowflake_t around, DisC_snowflake_t before, DisC_snowflake_t after, int limit, DisC_message_t *messages)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Getting channel messages");
}

short DisC_REST_GetChannelMessage(DisC_session_t *session, DisC_snowflake_t channelId, DisC_snowflake_t messageId, DisC_message_t *message)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Getting channel message");
}

short DisC_REST_CreateMessage(DisC_session_t *session, DisC_snowflake_t channelId, char *content, DisC_snowflake_t nonce, DisC_BOOL_t tts, unsigned char *fileData, unsigned long fileDataLen, DisC_embed_t *embed)//get channel id and add parameters
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Creating message");
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


  char *finalData = malloc(readDataLen + 1);//turn it into a string
  if(finalData == NULL)
  {
    exit(1);
  }
  memmove(finalData, readData, readDataLen);
  //finalData[readDataLen + 1] = 0x00;//terminate string
  finalData[readDataLen] = 0x00;//terminate string


  char *code = internal_GetHTTPResponseCode(session, finalData);


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
    DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_WARNING, "HTTP error: %s: %s", code, strstr(finalData, "\r\n\r\n"));
    DisC_AddError(session, DISC_ERROR_GENERIC);
    returnval = DISC_ERROR_GENERIC;
  }

  //free everything==================================================
  json_decref(root);
  free(code);
  free(base);
  free(url);
  free(final);
  free(readData);
  free(finalData);

  return returnval;
}

short DisC_REST_CreateReaction(DisC_session_t *session, DisC_snowflake_t channelId, DisC_snowflake_t messageId, DisC_emoji_t *emoji)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Creating reaction");
  //204 on success
}

short DisC_REST_DeleteOwnReaction(DisC_session_t *session, DisC_snowflake_t channelId, DisC_snowflake_t messageId, DisC_emoji_t *emoji)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Deleting own reaction");
  //204 on success
}

short DisC_REST_DeleteUserReaction(DisC_session_t *session, DisC_snowflake_t channelId, DisC_snowflake_t messageId, DisC_emoji_t *emoji, DisC_snowflake_t userId)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Deleting user reaction");
  //204 on success
}

short DisC_REST_GetReactions(DisC_session_t *session, DisC_snowflake_t channelId, DisC_snowflake_t messageId, DisC_emoji_t *emoji, DisC_user_t *users)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Getting reactions");
  //use GET query string
}

short DisC_REST_DeleteAllReactions(DisC_session_t *session, DisC_snowflake_t channelId, DisC_snowflake_t messageId)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Deleting all reactions");
  //assumin 204 is good
}

short DisC_REST_EditMessage(DisC_session_t *session, DisC_snowflake_t channelId, DisC_snowflake_t messageId)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Editing message");
  //basically the same as create message
}

short DisC_REST_DeleteMessage(DisC_session_t *session, DisC_snowflake_t channelId, DisC_snowflake_t messageId)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Deleting message");
  //204 on success
}

short DisC_REST_BulkDeleteMessages(DisC_session_t *session, DisC_snowflake_t channelId)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Bulk deleting messages");
  //204 on success
}

short DisC_REST_EditChannelPermissions(DisC_session_t *session, DisC_snowflake_t channelId, DisC_snowflake_t overwriteId)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Editing channel permissions");
  //204 on success
}

short DisC_REST_GetChannelInvites(DisC_session_t *session, DisC_snowflake_t channelId, DisC_invite_channel_t *invites)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Getting channel invites");
  //invites on success
}

short DisC_REST_CreateChannelInvite(DisC_session_t *session, DisC_snowflake_t channelId, DisC_invite_channel_t *invite)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Getting channel invite");
  //returns invite object
}

short DisC_REST_DeleteChannelPermission(DisC_session_t *session, DisC_snowflake_t channelId, DisC_snowflake_t overwriteId)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Deleting channel permission");
  //204 on success
}

short DisC_REST_TriggerTypingIndicator(DisC_session_t *session, DisC_snowflake_t channelId)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Triggering typing indicator");
  //204 on sucess
}

short DisC_REST_GetPinnedMessages(DisC_session_t *session, DisC_snowflake_t channelId, DisC_message_t *messages)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Getting pinned messages");
  //returns array of message objects
}

short DisC_REST_AddPinnedChannelMessage(DisC_session_t *session, DisC_snowflake_t channelId, DisC_snowflake_t messageId)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Getting pinned message");
  //204 on success
}

short DisC_REST_DeletePinnedChannelMessage(DisC_session_t *session, DisC_snowflake_t channelId, DisC_snowflake_t messageId)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Deleting pinned message");
  //204 in success
}

short DisC_REST_GroupDMAddRecipient(DisC_session_t *session, DisC_snowflake_t channelId, DisC_snowflake_t userId)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Group DM adding recipient");
  //maybe 200 on success
}

short DisC_REST_GroupDMRemoveRecipient(DisC_session_t *session, DisC_snowflake_t channelId, DisC_snowflake_t userId)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Group DM removing recipient");
  //maybe 204 on success
}

//end of channel REST functions
