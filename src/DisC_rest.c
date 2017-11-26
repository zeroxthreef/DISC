#include <stdlib.h>
#include <inttypes.h>
#include <ctype.h>

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/x509.h>

#include <jansson.h>

#include "DisC_types.h"
#include "DisC_util.h"
#include "DisC_errors.h"
#include "DisC_object.h"
#include "DisC_rest.h"

//TODO make an actual REST request function

char *internal_UnChunk(DisC_session_t *session, char *str);

char *internal_GetHTTPResponseCode(DisC_session_t *session, char *str);

char *internal_GenerateHeaders(DisC_session_t *session, char *url, char *httpAction, const char *contentType, unsigned long contentLength);

unsigned char *internal_ReadData(DisC_session_t *session, unsigned long *dataLen);

short internal_WriteData(DisC_session_t *session, unsigned char *data, unsigned long dataLen);

//======================================================================================

char *internal_UnChunk(DisC_session_t *session, char *str)
{
  char *returnStr = NULL;
  char *returnStrFinal = NULL;
  char *start = strstr(str, "\r\n\r\n");
  start += strlen("\r\n\r\n");
  char *scanStr = DisC_strmkdup(start);
  char *chunkToken = scanStr;
  DisC_BOOL_t foundChunkLen = DISC_FALSE;
  long chunkLen = 0;
  long chunkCount = 0;
  DisC_BOOL_t canExit = 0;


  if(strstr(str, "Transfer-Encoding") != NULL)
  {
    strtok(chunkToken, "\r\n");
    //printf("Token: %s\n", chunkToken);
    chunkLen = strtol(chunkToken, NULL, 16);
    foundChunkLen = DISC_TRUE;


    if(chunkLen > 0)
    {
      //returnStr = malloc(size_t __size)
      while((chunkToken = strtok(NULL, "\r\n")) != NULL && !canExit)
      {
        if(foundChunkLen)//if it found the num of chunk length, set up the body stuff
        {
          if((returnStr = realloc(returnStr, (chunkCount + chunkLen) * sizeof(char))) != NULL)
          {
            memmove(returnStr + chunkCount, chunkToken, chunkLen);
            chunkCount += chunkLen;
            foundChunkLen = DISC_FALSE;
          }
          else
            exit(1);
        }
        else
        {
          chunkLen = strtol(chunkToken, NULL, 16);
          if(chunkLen == 0)
          {
            canExit = 1;
          }
          foundChunkLen = DISC_TRUE;
        }
        //printf("Token: %s\n", chunkToken);
      }

      returnStrFinal = calloc(chunkCount + 1, sizeof(char));
      if(returnStrFinal == NULL)
        exit(1);

      memmove(returnStrFinal, returnStr, chunkCount);
    }

    //printf("Final: %s\n", returnStrFinal);
    free(scanStr);
    return returnStrFinal;
  }
  else
  {
    free(scanStr);
    return NULL;
  }


}

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
  char *base = "%s %s HTTP/1.1\r\nHost: %s\r\nConnection: keep-alive\r\nAuthorization: %s%s\r\nUser-Agent: (so sorry for not obeying rate limiting. Nearly at the point that I can add it)DisC v%d.%d.%d: %s\r\n%s\r\n%s";

  if(strcmp(httpAction, "GET") == 0 || strcmp(httpAction, "DELETE") == 0)
  {
    //otherwise just set the %s to nothing
    DisC_asprintf(&final, base, httpAction, url, DISC_REST_HOST, session->clientType ? "Bot " : "", session->token, DISC_VER_MAJOR, DISC_VER_MINOR, DISC_VER_REVISION, session->internalName, "", "");
  }
  else
  {
    //add a mime type and content length:
    //if(strcmp(httpAction, "GETQS") == 0)
    //{
      //DisC_asprintf(&final, base, "GET", url, host, session->clientType ? "Bot " : "", session->token, DISC_VER_MAJOR, DISC_VER_MINOR, DISC_VER_REVISION, session->internalName, "Content-Type: %s\r\nContent-Length: %lu\r\n", "%s");
    //}
    //else
    //{
    DisC_asprintf(&final, base, httpAction, url, DISC_REST_HOST, session->clientType ? "Bot " : "", session->token, DISC_VER_MAJOR, DISC_VER_MINOR, DISC_VER_REVISION, session->internalName, "Content-Type: %s\r\nContent-Length: %lu\r\n", "%s");
    //}

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
      char *tempScanBuffer = calloc(byteCount + 1, sizeof(char));
      memmove(tempScanBuffer, buffer, byteCount);
      if(strstr(tempScanBuffer, "\r\n0\r\n\r\n"))
      {
        //BIO_read(session->DONOTSET_rest_bio, NULL, strlen("0\r\n\r\n"));
        //printf("Found end of chunk: %d\n", status);
        chunkedCanExit = 1;
      }
      free(tempScanBuffer);
    }
    else if(contentLength > 0)
    {
      if(strstr(buffer, "\r\n\r\n") != NULL)
      {
        char *tempScanBuffer = calloc(byteCount + 1, sizeof(char));
        memmove(tempScanBuffer, buffer, byteCount);
        //printf("found end of headers\n");
        char *marked = strstr(tempScanBuffer, "\r\n\r\n");
        unsigned long i;
        for(i = 0; i < byteCount; i++)
        {
          if(&tempScanBuffer[i] == marked)
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
        free(tempScanBuffer);
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
    DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Reconnecting to %s..", DISC_REST_HOST);
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

short DisC_REST_InitSession(DisC_session_t *session)//TODO set a timeout
{
  //set the session ssl context to TLS 1.2
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Connecting to %s...", DISC_REST_HOST);
  session->DONOTSET_rest_ctx = SSL_CTX_new(TLSv1_2_client_method());


  session->DONOTSET_rest_bio = BIO_new_ssl_connect(session->DONOTSET_rest_ctx);
  BIO_set_conn_hostname(session->DONOTSET_rest_bio, DISC_REST_HOSTNAME);

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
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Disconnecting from %s...", DISC_REST_HOST);
  BIO_free_all(session->DONOTSET_rest_bio);
  SSL_CTX_free(session->DONOTSET_rest_ctx);

  DisC_AddError(session, DISC_ERROR_NONE);
  return DISC_ERROR_NONE;
}

short DisC_REST_DiscordHTTPRequest(DisC_session_t *session, char **returnBody, unsigned long *returnBodyLen, char **returnCode, char *method, char *mime, char *URI, char *data, unsigned long datalen)
{
  char *request = NULL;
  char *recieve = NULL;
  char *callocRequest = NULL;

  unsigned long internalRecieveLen = 0;

  if(strcmp(method, "GET") == 0 && strcmp(method, "DELETE") == 0)
  {
    request = internal_GenerateHeaders(session, URI, method, NULL, 0);
  }
  else
  {
    request = internal_GenerateHeaders(session, URI, method, mime, datalen);
    DisC_asprintf(&request, request, data);
  }

  do
  {
    internal_WriteData(session, request, strlen(request));
    recieve = internal_ReadData(session, &internalRecieveLen);
  }
  while(recieve == NULL);

  callocRequest = malloc(internalRecieveLen + 1);//turn it into a string
  if(callocRequest == NULL)
  {
    exit(1);
  }
  memmove(callocRequest, recieve, internalRecieveLen);

  *returnBody = internal_UnChunk(session, callocRequest);

  *returnCode = internal_GetHTTPResponseCode(session, callocRequest);

  *returnBodyLen = internalRecieveLen;

  if(*returnBody == NULL)//then its content length
  {
    *returnBody = DisC_strmkdup(strstr(callocRequest, "\r\n\r\n") + strlen("\r\n\r\n"));
  }




  free(request);
  free(recieve);
  free(callocRequest);

  DisC_AddError(session, DISC_ERROR_NONE);
  return DISC_ERROR_NONE;
}

//=============================================================================================================================================

short DisC_REST_GetChannel(DisC_session_t *session, DisC_snowflake_t channelId, DisC_channel_t **channel)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Getting channel");
  short returnval;
  char *url = NULL;
  char *code = NULL;
  char *readData = NULL;
  unsigned long readDataLen = 0;
  DisC_channel_t *channelInternal = NULL;

  DisC_asprintf(&url, "%s/channels/%s", DISC_REST_APIBASE, channelId);
  DisC_REST_DiscordHTTPRequest(session, &readData, &readDataLen, &code, "GET", NULL, url, NULL, 0);

  json_t *retRoot = NULL;


  //============================================

  if(strcmp(code, "200") == 0)
  {
    channelInternal = DisC_object_GenerateChannel(session, readData);

    if(channelInternal != NULL)
    {
      *channel = channelInternal;

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
    DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_WARNING, "HTTP error: %s: %s", code, readData);
    DisC_AddError(session, DISC_ERROR_GENERIC);
    returnval = DISC_ERROR_GENERIC;
  }


  free(code);
  free(readData);
  free(url);

  return returnval;
}

short DisC_REST_ModifyChannel(DisC_session_t *session, DisC_snowflake_t channelId, char *name, int position, char *topic, DisC_BOOL_t nsfw, int bitrate, int userLimit, DisC_overwrite_t *overwrites, DisC_snowflake_t parentId)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Modifying channel");
  short returnval;
  char *url = NULL;
  char *code = NULL;
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

  DisC_asprintf(&url, "%s/channels/%s", DISC_REST_APIBASE, channelId);
  DisC_REST_DiscordHTTPRequest(session, &readData, &readDataLen, &code, "PUT", "application/json", url, json_dumps(root, 0), strlen(json_dumps(root, 0)));


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
    DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_WARNING, "HTTP error: %s: %s", code, readData);
    DisC_AddError(session, DISC_ERROR_GENERIC);
    returnval = DISC_ERROR_GENERIC;
  }

  //free everything==================================================
  json_decref(root);
  free(code);
  free(url);
  free(readData);

  return returnval;
}

short DisC_REST_DeleteChannel(DisC_session_t *session, DisC_snowflake_t channelId)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Deleting channel");
  short returnval;
  char *url = NULL;
  char *code = NULL;
  char *readData = NULL;
  unsigned long readDataLen = 0;

  DisC_asprintf(&url, "%s/channels/%s", DISC_REST_APIBASE, channelId);

  DisC_REST_DiscordHTTPRequest(session, &readData, &readDataLen, &code, "DELETE", NULL, url, NULL, 0);


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
    DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_WARNING, "HTTP error: %s: %s", code, readData);
    DisC_AddError(session, DISC_ERROR_GENERIC);
    returnval = DISC_ERROR_GENERIC;
  }

  //free everything==================================================
  free(code);
  free(url);
  free(readData);

  return returnval;
}

short DisC_REST_GetChannelMessages(DisC_session_t *session, DisC_snowflake_t channelId, DisC_snowflake_t around, DisC_snowflake_t before, DisC_snowflake_t after, int limit, DisC_message_t **messages, unsigned long *messageNum)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Getting channel messages");
  short returnval;
  char *queryString = NULL;
  char *url = NULL;
  char *readData = NULL;
  char *code = NULL;
  unsigned long readDataLen = 0;

  DisC_message_t *messagesInternal = NULL;


  DisC_asprintf(&queryString, "?");
  if(around != NULL)
    DisC_asprintf(&queryString, "%saround=%s", queryString, around);

  if(before != NULL)
    DisC_asprintf(&queryString, "%sbefore=%s", queryString, before);

  if(after != NULL)
    DisC_asprintf(&queryString, "%safter=%s", queryString, after);

  if(limit > 0)//from discord docs: "	max number of messages to return (1-100)" so 0 should be good for setting to nothing
    DisC_asprintf(&queryString, "%s&limit=%d", queryString, limit);


  //DisC_asprintf(&queryString, "?around=%s&before=%s&after=%s&limit=%d", around, before, after, limit);
  DisC_asprintf(&url, "%s/channels/%s/messages%s", DISC_REST_APIBASE, channelId, queryString);

  DisC_REST_DiscordHTTPRequest(session, &readData, &readDataLen, &code, "GET", NULL, url, NULL, 0);

  //json stuff
  json_t *retRoot = NULL;
  //============================================

  if(strcmp(code, "200") == 0)
  {

    if(readData != NULL)
    {
      //printf("Parsing JSON\n");
      unsigned long i;
      retRoot = json_loads(readData, 0, NULL);

      //printf("JSON: %s\n", json_dumps(retRoot, JSON_INDENT(2)));
      //TODO count array elements and allocate a list of messages based on array size

      messagesInternal = calloc(json_array_size(retRoot), sizeof(DisC_message_t));
      if(messagesInternal == NULL)
        exit(1);

      for(i = 0; i < json_array_size(retRoot); i++)
      {
        //printf("%s\n", json_dumps(json_array_get(retRoot, i), 0));
        DisC_message_t *messageInternal = DisC_object_GenerateMessage(session, json_dumps(json_array_get(retRoot, i), 0));
        //messagesInternal[i] = *messageInternal;
        //printf("%s\n", messageInternal->content);
        //memmove(&messagesInternal[i], messageInternal, sizeof(DisC_message_t));
        messagesInternal[i] = *messageInternal;
        //printf("%s\n", messagesInternal[i].content);
        free(messageInternal);//TODO actually delete it properly
      }
      //set stuff
      *messageNum = json_array_size(retRoot);
      *messages = messagesInternal;

      //printf("last test: %s\n", messages[i]->content);

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
    DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_WARNING, "HTTP error: %s: %s", code, readData);
    DisC_AddError(session, DISC_ERROR_GENERIC);
    returnval = DISC_ERROR_GENERIC;
  }
  json_decref(retRoot);
  free(queryString);
  free(code);
  free(readData);
  free(url);

  return returnval;
}

short DisC_REST_GetChannelMessage(DisC_session_t *session, DisC_snowflake_t channelId, DisC_snowflake_t messageId, DisC_message_t **message)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Getting channel message");
  short returnval;
  char *url = NULL;
  char *code = NULL;
  char *readData = NULL;
  unsigned long readDataLen = 0;
  DisC_message_t *messageInternal = NULL;

  DisC_asprintf(&url, "%s/channels/%s/messages/%s", DISC_REST_APIBASE, channelId, messageId);
  DisC_REST_DiscordHTTPRequest(session, &readData, &readDataLen, &code, "GET", NULL, url, NULL, 0);


  //============================================

  if(strcmp(code, "200") == 0)
  {
    messageInternal = DisC_object_GenerateMessage(session, readData);

    if(messageInternal != NULL)
    {
      *message = messageInternal;

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
    DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_WARNING, "HTTP error: %s: %s", code, readData);
    DisC_AddError(session, DISC_ERROR_GENERIC);
    returnval = DISC_ERROR_GENERIC;
  }


  free(code);
  free(readData);
  free(url);

  return returnval;
}

short DisC_REST_CreateMessage(DisC_session_t *session, DisC_snowflake_t channelId, char *content, DisC_snowflake_t nonce, DisC_BOOL_t tts, unsigned char *fileData, unsigned long fileDataLen, DisC_embed_t *embed)//get channel id and add parameters
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Creating message");
  char *url = NULL;
  char *readData = NULL;
  char *code = NULL;
  unsigned long readDataLen = 0;
  short returnval;

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

  DisC_asprintf(&url, "%s/channels/%s/messages", DISC_REST_APIBASE, channelId);
  if(fileData != NULL)
  {
    DisC_REST_DiscordHTTPRequest(session, &readData, &readDataLen, &code, "POST", "multipart/form-data", url, json_dumps(root, 0), strlen(json_dumps(root, 0)));
    //TODO make it have the parts when sending files
  }
  else
  {
    DisC_REST_DiscordHTTPRequest(session, &readData, &readDataLen, &code, "POST", "application/json", url, json_dumps(root, 0), strlen(json_dumps(root, 0)));
  }
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
    DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_WARNING, "HTTP error: %s: %s", code, readData);
    DisC_AddError(session, DISC_ERROR_GENERIC);
    returnval = DISC_ERROR_GENERIC;
  }

  //free everything==================================================
  json_decref(root);
  free(code);
  free(url);
  free(readData);

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
