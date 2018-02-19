#ifndef ERRORS_H__
#define ERRORS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "DisC_types.h"

enum DisC_Errors
{
  DISC_ERROR_NONE,
  DISC_ERROR_GENERIC,
  DISC_ERROR_PERMISSIONS,
  DISC_ERROR_API_ERROR,
  DISC_ERROR_CONNECTION,
  DISC_ERROR_OBJECT_NONEXISTANT,
  DISC_ERROR_CONTENT_TOO_LARGE,
  DISC_ERROR_MEMORY,
};

void DisC_PrintError(DisC_session_t *session);

const char *DisC_GetSessionErrorString(DisC_session_t *session);

const char *DisC_GetErrorString(unsigned short error);

void DisC_AddError(DisC_session_t *session, int error);

#ifdef __cplusplus
}
#endif

#endif /* ERRORS_H__*/
