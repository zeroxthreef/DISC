const char *DISCORD_REST_BASE_URL = "https://discordapp.com/api"; //this should never change unless discord does. thanks boysanic

int DISC_REST_GetChannel(Channel *channel, snowflake channelId);

int DISC_REST_ModifyChannel(Channel *channel, snowflake channelId);
