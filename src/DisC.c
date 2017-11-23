#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DisC.h"

/*The only global variables in DisC*/
DisC_session_t *sessions = NULL;

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

  DisC_REST_InitSession(session);


  DisC_AddError(session, DISC_ERROR_NONE);
  return DISC_ERROR_NONE;
}

short DisC_DesroySession(DisC_session_t *session)
{
  DisC_Log(session->logLevel, session->logFileLocation, DISC_SEVERITY_NOTIFY, "Ending session %s", session->internalName);

  DisC_REST_DestroySession(session);

  DisC_AddError(session, DISC_ERROR_NONE);
  return DISC_ERROR_NONE;
}

short DisC_StartAllSessions()
{



  return DISC_ERROR_NONE;
}
