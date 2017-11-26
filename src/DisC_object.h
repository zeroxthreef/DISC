#ifndef OBJECT_H__
#define OBJECT_H__

#ifdef __cplusplus
extern "C" {
#endif

DisC_channel_t *DisC_object_GenerateChannel(DisC_session_t *session, char *jsonData);

DisC_message_t *DisC_object_GenerateMessage(DisC_session_t *session, char *jsonData);

DisC_user_t *DisC_object_GenerateUser(DisC_session_t *session, char *jsonData);

DisC_overwrite_t *DisC_object_GenerateOverwrite(DisC_session_t *session, char *jsonData);

DisC_role_t *DisC_object_GenerateRole(DisC_session_t *session, char *jsonData);

DisC_attachment_t *DisC_object_GenerateAttachment(DisC_session_t *session, char *jsonData);

DisC_embed_t *DisC_object_GenerateEmbed(DisC_session_t *session, char *jsonData);

DisC_reaction_t *DisC_object_GenerateReaction(DisC_session_t *session, char *jsonData);

#ifdef __cplusplus
}
#endif

#endif /* OBJECT_H__*/
