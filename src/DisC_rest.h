#ifndef REST_H__
#define REST_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "DisC_types.h"

short DisC_REST_Init();

short DisC_REST_ReconnectSession(DisC_session_t *session);

short DisC_REST_InitSession(DisC_session_t *session);

short DisC_REST_DestroySession(DisC_session_t *session);

//==============================================================

short DisC_REST_GetChannel(DisC_session_t *session, DisC_snowflake_t channelId, DisC_channel_t *channel);

short DisC_REST_CreateMessage(DisC_session_t *session, DisC_snowflake_t channelId, char *content, DisC_snowflake_t nonce, DisC_BOOL_t tts, unsigned char *fileData, unsigned long fileDataLen, DisC_embed_t *embed);

#ifdef __cplusplus
}
#endif

#endif /* REST_H__*/
