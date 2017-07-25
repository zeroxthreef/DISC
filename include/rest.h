const char *DISCORD_REST_BASE_URL = "discordapp.com/api"; //this should never change unless discord does. thanks boysanic

int DISC_REST_GetChannel(snowflake channelID, Channel *channel);

int DISC_REST_ModifyChannel(snowflake channelID, char *name, int position, char *topic, int bitrate, int user_limit);

int DISC_REST_DeleteChannel(snowflake channelID);

int DISC_REST_GetChannelMessages(snowflake channelID, snowflake around, snowflake before, snowflake after, int limit, Message *messages);

int DISC_REST_GetChannelMessage(snowflake channelID, snowflake messageID, Message *message);

int DISC_REST_CreateMessage(snowflake channelID, char *content, snowflake nonce, BOOL tts, unsigned char *data, Embed embed);

int DISC_REST_CreateReaction(snowflake channelID, snowflake messageID, Emoji emoji);

int DISC_REST_DeleteReaction(snowflake channelID, snowflake messageID, Emoji emoji);

int DISC_REST_DeleteUserReaction(snowflake channelID, snowflake messageID, snowflake userID, Emoji emoji);

int DISC_REST_GetReactions(User *users, snowflake channelID, snowflake messageID, Emoji emoji);

int DISC_REST_DeleteAllReactions(snowflake channelID, snowflake messageID);

int DISC_REST_EditMessage(snowflake channelID, snowflake messageID, char *content, Embed embed);

int DISC_REST_DeleteMessage(snowflake channelID, snowflake messageID);

int DISC_REST_BulkDeleteMessages(snowflake channelID, snowflake *messageIDs);

int DISC_REST_EditChannelPermissions(snowflake channelID, snowflake overwriteID, int allow, int deny, char *type);

int DISC_REST_GetChannelInvites(snowflake channelID, Invite *invite);

int DISC_REST_CreateChannelInvite(snowflake channelID, Invite *invite, int max_age, int max_uses, BOOL temporary, BOOL unique);

int DISC_REST_DeleteChannelPermission(snowflake channelID, snowflake overwriteID);

int DISC_REST_TriggerTypingIndicator(snowflake channelID);

int DISC_REST_GetPinnedMessages(snowflake channelID, Message *messages);

int DISC_REST_AddPinnedChannelMessage(snowflake channelID, snowflake messageID);

int DISC_REST_DeletePinnedChannelMessage(snowflake channelID, snowflake messageID);

int DISC_REST_GroupDMAddRecipient(snowflake channelID, snowflake userID, char *access_token, char *nick);

int DISC_REST_GroupDMRemoveRecipient(snowflake channelID, snowflake userID);
