#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#include <jansson.h>

#include "DisC_types.h"
#include "DisC_util.h"
#include "DisC_errors.h"
#include "DisC_object.h"
#include "DisC_rest.h"
#include "DisC_gateway.h"



static char *internal_GetHTTPResponseCode(DisC_session_t *session, char *str);

static char *internal_GenerateUpgradeHeaders(DisC_session_t *session, char *b64Key, char *URI, char *host);

static unsigned char *internal_ReadData(DisC_session_t *session, unsigned long *dataLen, DisC_BOOL_t isHandshaking);

static short internal_WriteData(DisC_session_t *session, unsigned char *data, unsigned long dataLen);

//=======================================================================================================

short internal_CheckSendHeartbeat(DisC_session_t *session)//TODO completely redo the networking system so long blocking operations don't interrupt heartbeats
{
  #ifdef _WIN32

  #else
  struct timespec *tick;
  tick = (struct timespec *)malloc(sizeof(tick));
  clock_gettime(CLOCK_MONOTONIC, &tick);
  #endif
  //check last tick heartbeat was sent at
  //if tick is after the tick wait, send a heartbeat, then check for opcode 11
  //otherwise, don' do anything
}

static char *internal_GetHTTPResponseCode(DisC_session_t *session, char *str)
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

static char *internal_GenerateUpgradeHeaders(DisC_session_t *session, char *b64Key, char *URI, char *host)
{
  char *finalString = NULL;
  char *base = "GET %s?v=6&encoding=json HTTP/1.1\r\nHost: %s\r\nConnection: keep-alive\r\nAuthorization: %s%s\r\nUser-Agent: (so sorry for not obeying rate limiting. Nearly at the point that I can add it)DisC v%d.%d.%d: %s\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-webSocket-Key: %s\r\nSec-WebSocket-Protocol: chat, superchat\r\nSec-WebSocket-Version: 13\r\n\r\n";
  DisC_asprintf(&finalString, base, URI, host, session->clientType ? "Bot " : "", session->token, DISC_VER_MAJOR, DISC_VER_MINOR, DISC_VER_REVISION, session->internalName, b64Key);

  //char *base = "GET %s HTTP/1.1\r\nHost: %s\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-webSocket-Key: %s\r\nSec-WebSocket-Protocol: chat, superchat\r\nSec-WebSocket-Version: 13\r\n\r\n";
  //DisC_asprintf(&finalString, base, URI, host, b64Key);


  return finalString;
}


static unsigned char *internal_ReadData(DisC_session_t *session, unsigned long *dataLen, DisC_BOOL_t isHandshaking)
{
  char *buffer = NULL;
  unsigned long byteCount = 0;
  unsigned long byteChunkMax = 1024;
  unsigned long contentLength = 0;
  unsigned long status;
  int foundHTTPEnd = 0;

  buffer = malloc(byteChunkMax);
  if(buffer == NULL)
  {
    exit(1);
  }
  status = byteChunkMax;
  do {
    status = (unsigned long)BIO_read(session->DONOTSET_gateway_bio, buffer + byteCount, status);
    if(status == -1)
    {
      return NULL;
    }

    byteCount += status;

    buffer = realloc(buffer, byteCount + byteChunkMax);
    //printf("%d\n", status);


    //printf("buffer: %s\nstatus: %lu", buffer, status);
    if(strstr(buffer, "\r\n\r\n") != NULL && isHandshaking)
    {
      foundHTTPEnd = 1;
    }
    else if(strchr(buffer, 0x00))
    {
      foundHTTPEnd = 1;//reusing variable
    }



  } while(status > 0 && !foundHTTPEnd);




  buffer = realloc(buffer, byteCount);

  *dataLen = byteCount;
  return buffer;
}

static short internal_WriteData(DisC_session_t *session, unsigned char *data, unsigned long dataLen)
{
  if(BIO_write(session->DONOTSET_gateway_bio, data, dataLen) <= 0)
  {
    DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Couldnt send to gateway... At the moment, this means further library dysfunction because I dont want to risk being banned from the api if I pull something stupid on accident");
    DisC_AddError(session, DISC_ERROR_CONNECTION);
    return DISC_ERROR_CONNECTION;
  }

  DisC_AddError(session, DISC_ERROR_NONE);
  return DISC_ERROR_NONE;
}


short DisC_gateway_InitSession(DisC_session_t *session, DisC_callbacks_t *callbacks)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Starting session %s gateway", session->internalName);
  short returnval;
  char *url = NULL;
  char *code = NULL;
  char *readData = NULL;
  unsigned long readDataLen = 0;

  DisC_asprintf(&url, session->clientType ? "%s/gateway/bot" : "%s/gateway", DISC_REST_APIBASE);
  DisC_REST_DiscordHTTPRequest(session, &readData, &readDataLen, &code, "GET", NULL, url, NULL, 0);


  char *replace = strstr(readData, "}");
  replace++;
  *replace = '\0';

  json_t *retRoot = NULL;

  //============================================

  if(strcmp(code, "200") == 0)
  {
    retRoot = json_loads(readData, 0, NULL);

    if(retRoot != NULL)
    {
      //make a nonblocking socket with ssl stuff
      unsigned long retries = 0;
      int result;
      char *wsurl = json_string_value(json_object_get(retRoot, "url"));
      wsurl += strlen("wss://");
      char *finalWSURL = NULL;
      DisC_asprintf(&finalWSURL, "%s:https", wsurl);

      //printf()
      //===========================================================


      session->DONOTSET_gateway_ctx = SSL_CTX_new(TLSv1_2_client_method());


      session->DONOTSET_gateway_bio = BIO_new_ssl_connect(session->DONOTSET_gateway_ctx);
      BIO_set_conn_hostname(session->DONOTSET_gateway_bio, finalWSURL);
      BIO_set_nbio(session->DONOTSET_gateway_bio, 1);
      result = BIO_do_connect(session->DONOTSET_gateway_bio);
      if(result <= 0)
      {
        while(BIO_should_retry(session->DONOTSET_gateway_bio) && retries != DISC_MAX_RECONNECTS)
        {
          DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_ERROR, "Waiting for socket");
          result = BIO_do_connect(session->DONOTSET_gateway_bio);
          if(result > 0)
            break;


          sleep(1);//set to one second because this is the only one that actually works for some reason. BIO non blocking is really weird.
          retries++;
        }
      }
      else
      {
        DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Connected to gateway first try");
        DisC_AddError(session, DISC_ERROR_NONE);
        returnval = DISC_ERROR_NONE;
      }

      if(result <= 0)
      {
        DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Could not connect to gateway after retrying max times or there was a serious error");
        DisC_AddError(session, DISC_ERROR_CONNECTION);
        returnval = DISC_ERROR_CONNECTION;
      }
      else
      {
        DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Handshaking with gateway");
        //do everything to make a websocket tcp Connection
        unsigned char *returnData = NULL;
        unsigned long returnDataLen = 0;
        char *jsonResponse = NULL;
        char *request = internal_GenerateUpgradeHeaders(session, "x3JJHMbDL1EzLkh9GBhXDw==", "/", wsurl);//TODO not use the wikipedia base64 haha
        internal_WriteData(session, request, strlen(request));
        returnData = internal_ReadData(session, &returnDataLen, DISC_TRUE);
        while(returnData == NULL && (errno == EAGAIN || errno == EWOULDBLOCK))
        {
          returnData = internal_ReadData(session, &returnDataLen, DISC_TRUE);
          sleep(1);
        }

        //printf("http data: %s\n", returnData);

        code = internal_GetHTTPResponseCode(session, returnData);
        free(returnData);

        if(strcmp(code, "101") == 0)
        {
          DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Gateway handshake success");
          DisC_AddError(session, DISC_ERROR_NONE);
          returnval = DISC_ERROR_NONE;
          DisC_gateway_payload_t *internalPayload = NULL;

          //parse json now
          returnData = internal_ReadData(session, &returnDataLen, DISC_FALSE);
          while(returnData == NULL && (errno == EAGAIN || errno == EWOULDBLOCK))
          {
            returnData = internal_ReadData(session, &returnDataLen, DISC_FALSE);
            sleep(1);
          }
          jsonResponse = strstr(returnData, "{");
          char *replace = strrchr(jsonResponse, '}');
          replace++;
          *replace = '\0';
          //printf("json data: %s\n", jsonResponse);

          internalPayload = DisC_object_GenerateGatewayPayload(session, jsonResponse);

          printf("opcode: %d data: %s\n", internalPayload->op, internalPayload->d);
          if(internalPayload->op == DISC_OP_HELLO)
          {
            json_t *hello = json_loads(internalPayload->d, 0, NULL);
            session->DONOTSET_heartbeat_interval = json_integer_value(json_object_get(hello, "heartbeat_interval"));
            printf("Heartbeat interval: %d\n", session->DONOTSET_heartbeat_interval);
            json_decref(hello);
            //session->DONOTSET_heartbeat_interval = internalPayload->

            //send first heartbeat
          }
          else
          {
            //error
          }


        }
        else
        {
          DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Could not handshake with gateway");
          DisC_AddError(session, DISC_ERROR_CONNECTION);
          returnval = DISC_ERROR_CONNECTION;
        }


        free(request);
        DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Connected to gateway");
        DisC_AddError(session, DISC_ERROR_NONE);
        returnval = DISC_ERROR_NONE;
      }

      free(finalWSURL);
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

short DisC_gateway_DestroySession(DisC_session_t *session)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Disconnecting %s from gateway...", session->internalName);
  BIO_free_all(session->DONOTSET_gateway_bio);
  SSL_CTX_free(session->DONOTSET_gateway_ctx);
}

short DisC_gateway_ListenAndManage(DisC_session_t *session)
{

}
