#include <stdio.h>
#include <stdlib.h>

#include <jansson.h>

#include "DisC_types.h"
#include "DisC_util.h"
#include "DisC_errors.h"
#include "DisC_object.h"
#include "DisC_rest.h"
#include "DisC_gateway.h"


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
          DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_ERROR, "Reconnecting to gateway");
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
        DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Connected to gateway");
        DisC_AddError(session, DISC_ERROR_NONE);
        returnval = DISC_ERROR_NONE;
      }


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
