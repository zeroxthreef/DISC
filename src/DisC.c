#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "DisC.h"

/*The only global variables in DisC*/
DisC_session_t **sessions = NULL;

unsigned long sessionCount = 0;
/*===================================================*/

short DisC_InitLibrary()
{
  DisC_REST_Init();

  return DISC_ERROR_NONE;
}

short DisC_DestroyLibrary()
{



  return DISC_ERROR_NONE;
}

short DisC_CreateSession(DisC_session_t *session, DisC_callbacks_t *callbacks)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Starting session %s", session->internalName);

  DisC_REST_InitSession(session);//TODO do error checking

  DisC_gateway_InitSession(session, callbacks);//TODO do error checking


  //TODO add session to array of sessions
  //TODO add callbacks to array of callbacks

  if(sessionCount == 0)
  {
    sessions = malloc(sizeof(DisC_session_t *));
    if(sessions == NULL)
    {
      exit(1);
    }
  }
  else
  {
    sessions = realloc(sessions, sizeof(DisC_session_t *));
    if(sessions == NULL)//TODO replace every one of these exits with error messages
    {
      exit(1);
    }
  }

  sessions[sessionCount] = session;
  sessionCount++;


  DisC_AddError(session, DISC_ERROR_NONE);
  return DISC_ERROR_NONE;
}

short DisC_DesroySession(DisC_session_t *session)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Ending session %s", session->internalName);

  DisC_gateway_DestroySession(session);//destroy in the opposite order from createSession

  DisC_REST_DestroySession(session);//TODO generally check errors on everything

  //TODO actually remove pointer from array of session pointers
  sessionCount--;

  DisC_AddError(session, DISC_ERROR_NONE);
  return DISC_ERROR_NONE;
}

short DisC_StartAllSessions()
{
  //DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Entering loop");
  printf("Enterring loop\n");
  //TODO make a custom loop that runs in threads
  unsigned long i;
  while(sessionCount > 0)
  {
    //loop through all discord gateway sockets and check for data. TODO use select or some polling thing.

    for(i = 0; i < sessionCount; i++)
    {
      DisC_gateway_ListenAndManage(sessions[i]);
    }

    DisC_Delay(500);//delay half a second

    //call a post loop function
  }

  return DISC_ERROR_NONE;
}
