#ifndef GATEWAY_H__
#define GATEWAY_H__

#ifdef __cplusplus
extern "C" {
#endif

short DisC_gateway_InitSession(DisC_session_t *session, DisC_callbacks_t *callbacks);

short DisC_gateway_DestroySession(DisC_session_t *session);

short DisC_gateway_ListenAndManage(DisC_session_t *session);

short DisC_gateway_SendEvent(DisC_session_t *session, short OP, unsigned char *data, unsigned long dataLen);

short DisC_gateway_Identify(DisC_session_t *session, short large_threshold, short shardnum, short shardmax, DisC_gateway_status_update_t *status_update);

#ifdef __cplusplus
}
#endif

#endif /* GATEWAY_H__*/
