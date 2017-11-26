#ifndef GATEWAY_H__
#define GATEWAY_H__

#ifdef __cplusplus
extern "C" {
#endif

short DisC_gateway_InitSession(DisC_session_t *session, DisC_callbacks_t *callbacks);

short DisC_gateway_DestroySession(DisC_session_t *session);

short DisC_gateway_ListenAndManage(DisC_session_t *session);

#ifdef __cplusplus
}
#endif

#endif /* GATEWAY_H__*/
