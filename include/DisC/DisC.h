#ifndef DISC_H__
#define DISC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "DisC_types.h"
#include "DisC_errors.h"
#include "DisC_util.h"
#include "DisC_rest.h"
#include "DisC_object.h"
#include "DisC_gateway.h"


short DisC_InitLibrary();

short DisC_DestroyLibrary();

short DisC_CreateSession(DisC_session_t *session, DisC_callbacks_t *callbacks);

short DisC_DesroySession(DisC_session_t *session);

short DisC_StartAllSessions();



#ifdef __cplusplus
}
#endif

#endif /* DISC_H__*/
