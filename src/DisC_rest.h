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

short DisC_REST_ModifyChannel(DisC_session_t *session, DisC_snowflake_t channelId, char *name, int position, char *topic, DisC_BOOL_t nsfw, int bitrate, int userLimit, DisC_overwrite_t *overwrites, DisC_snowflake_t parentId);

short DisC_REST_DeleteChannel(DisC_session_t *session, DisC_snowflake_t channelId);

short DisC_REST_GetChennelMessages(DisC_session_t *session, DisC_snowflake_t channelId, DisC_snowflake_t around, DisC_snowflake_t before, DisC_snowflake_t after, int limit, DisC_message_t *messages);

short DisC_REST_GetChannelMessage(DisC_session_t *session, DisC_snowflake_t channelId, DisC_snowflake_t messageId, DisC_message_t *message);

short DisC_REST_CreateMessage(DisC_session_t *session, DisC_snowflake_t channelId, char *content, DisC_snowflake_t nonce, DisC_BOOL_t tts, unsigned char *fileData, unsigned long fileDataLen, DisC_embed_t *embed);

short DisC_REST_CreateReaction(DisC_session *session, DisC_snowflake_t channelId, DisC_snowflake_t messageId, DisC_emoji_t *emoji);

short DisC_REST_DeleteOwnReaction(DisC_session *session, DisC_snowflake_t channelId, DisC_snowflake_t messageId, DisC_emoji_t *emoji);

short DisC_REST_DeleteUserReaction(DisC_session *session, DisC_snowflake_t channelId, DisC_snowflake_t messageId, DisC_emoji_t *emoji, DisC_snowflake_t userId);

short DisC_REST_GetReactions(DisC_session *session, DisC_snowflake_t channelId, DisC_snowflake_t messageId, DisC_emoji_t *emoji, DisC_user_t *users);

short DisC_REST_DeleteAllReactions(DisC_session *session, DisC_snowflake_t channelId, DisC_snowflake_t messageId);

short DisC_REST_EditMessage(DisC_session *session, DisC_snowflake_t channelId, DisC_snowflake_t messageId);

short DisC_REST_DeleteMessage(DisC_session *session, DisC_snowflake_t channelId, DisC_snowflake_t messageId);

short DisC_REST_BulkDeleteMessages(DisC_session *session, DisC_snowflake_t channelId);

short DisC_REST_EditChannelPermissions(DisC_session *session, DisC_snowflake_t channelId, DisC_snowflake_t overwriteId);

short DisC_REST_GetChannelInvites(DisC_session *session, DisC_snowflake_t channelId, DisC_invite_channel_t *invites);

short DisC_REST_CreateChannelInvite(DisC_session *session, DisC_snowflake_t channelId, DisC_invite_channel_t *invite);

short DisC_REST_DeleteChannelPermission(DisC_session *session, DisC_snowflake_t channelId, DisC_snowflake_t overwriteId);

short DisC_REST_TriggerTypingIndicator(DisC_session *session, DisC_snowflake_t channelId);

short DisC_REST_GetPinnedMessages(DisC_session *session, DisC_snowflake_t channelId, DisC_message_t *messages);

short DisC_REST_AddPinnedChannelMessage(DisC_session *session, DisC_snowflake_t channelId, DisC_snowflake_t messageId);

short DisC_REST_DeletePinnedChannelMessage(DisC_session *session, DisC_snowflake_t channelId, DisC_snowflake_t messageId);

short DisC_REST_GroupDMAddRecipient(DisC_session *session, DisC_snowflake_t channelId, DisC_snowflake_t userId);

short DisC_REST_GroupDMRemoveRecipient(DisC_session *session, DisC_snowflake_t channelId, DisC_snowflake_t userId);

//===========

#ifdef __cplusplus
}
#endif

#endif /* REST_H__*/
