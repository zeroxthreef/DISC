#include <stdio.h>
#include <stdlib.h>

#include "DisC_errors.h"

const char *errors[] = {//never set so thread safe
  "Operation was successfull.\0",
  "There was a generic error. This is most likely not severe.\0",
  "The account has insufficient permissions to do this.\0",
  "The API server returned an error that isn't defined.\0",
  "The session has lost connection or not connected.\0"
  "The object requested doesnt exist.\0"
  "Either the message was too large or what was sent exceeded limits for Discord.\0"
  "Not enough ram or memory corruption.\0"
};


void DisC_PrintError(DisC_session_t *session)
{
  printf("%s\n", errors[session->DONOTSET_currentError]);
}

const char *DisC_GetSessionErrorString(DisC_session_t *session)
{
  return errors[session->DONOTSET_currentError];
}

const char *DisC_GetErrorString(unsigned short error)
{
  return errors[error];
}

void DisC_AddError(DisC_session_t *session, int error)
{
  session->DONOTSET_currentError = error;
}
