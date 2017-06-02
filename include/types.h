#include <stdint.h>

typedef uint64_t snowflake;
typedef uint8_t  BOOL;
typedef uint32_t sessionID; //no need for anything larger. No way more than uint sessions will be used ever


typedef struct{
  char *token;
  BOOL clientType;
  sessionID id;
} Session;


typedef void (*DISC_callback) (Session *session, void *object);



typedef struct{
  DISC_callback ready;
  DISC_callback resumed;
  DISC_callback channel_create;
  DISC_callback channel_update;
  DISC_callback channel_delete;
  DISC_callback guild_create;
  DISC_callback guild_update;
  DISC_callback guild_delete;
  DISC_callback guild_ban_add;
  DISC_callback guild_ban_remove;
  DISC_callback guild_emoji_update;
  DISC_callback guild_integrations_update;
  DISC_callback guild_member_add;
  DISC_callback guild_member_remove;
  DISC_callback guild_member_update;
  DISC_callback guild_members_chunk;
  DISC_callback guild_role_create;
  DISC_callback guild_role_update;
  DISC_callback guild_role_delete;
  DISC_callback message_create;
  DISC_callback message_update;
  DISC_callback message_delete;
  DISC_callback message_delete_bulk;
  DISC_callback presence_update;
  DISC_callback game_object;//this is an event?
  DISC_callback typing_start;
  DISC_callback user_update;
  DISC_callback voice_state_update;
  DISC_callback voice_server_update;
} Callbacks;

//discord objects------------------------------------------

typedef struct{
	snowflake id;
	char* type;
	int allow;
	int deny;
} Overwrite;

typedef struct{
  snowflake id;
  char *username;
  char *discriminator;
  char *avatar;
  BOOL bot;
  BOOL mfa_enabled;
  BOOL verified;
  char *email;
} User;

typedef struct{
  snowflake id;
  char *name;
  int color;
  BOOL hoist;
  int position;
  int permissions;
  BOOL managed;
  BOOL mentionable;
} Role;

typedef struct{
	char* url;
	char* proxy_url;
	int height;
	int width;
} Thumbnail;

typedef struct{
	char* url;
	int height;
	int width;
} Video;

typedef struct{
	char* url;
	char* proxy_url;
	int height;
	int width;
} Image;

typedef struct{
	char* name;
	char* url;
} Provider;

typedef struct{
	char* name;
	char* url;
	char* icon_url;
	char* proxy_icon_url;
} Author;

typedef struct{
	char* text;
	char* icon_url;
	char* proxy_icon_url;
} Footer;

typedef struct{
	snowflake id;
	char* filename;
	int size;
	char* url;
	char* proxy_url;
	int height;
	int width;
} Attachment;

typedef struct{
	snowflake id;
	char* name;
} Emoji;

typedef struct{
	int count;
	int me;
	Emoji emoji;
} Reaction;

typedef struct{
	char* name;
	char* value;
	int isInline;//crap, seems this is a keyword. Rename this. TODO TODO TODO TODO
} Field;

typedef struct{
	char* title;
	char* type;
	char* description;
	char* url;
	char* date;//lookup this format so it can be parsed
	int color;
	Footer footer;
	Image image;
	Thumbnail thumbnail;
	Video video;
	Provider provider;
	Author author;
	Field *fields; //TODO this is an array. needs malloc
} Embed;

typedef struct{
  User user;
  char *nick;
  Role *roles;
  char *joined_at;//parse this later. Its a datetime
  BOOL deaf;
  BOOL mute;
} Member;

typedef struct{
  char *id; //why isnt this a snowflake??
  char *name;
} Account;

typedef struct{
  char *name;
  int type;
  char *url;
} Game;

typedef struct{
  User user;
  snowflake *roles;
  Game game;
  snowflake guild_id;
  char *status;
} Presence;

typedef struct{
  snowflake id;
  char *name;
  char *type;
  BOOL enabled;
  BOOL syncing;
  snowflake role_id;
  int expire_behavior;
  int expire_grace_period;
  User user;
  Account account;
  char *synced_at; //cormat this later
} Integration;

typedef struct{
  snowflake guild_id;
  snowflake channel_id;
  snowflake user_id;
  char *sssion_id;
  BOOL deaf;
  BOOL mute;
  BOOL self_deaf;
  BOOL self_mute;
  BOOL suppress;
} Voice;

typedef struct{
	snowflake id;
	snowflake guild_id;
	char* name;
	char* type;//going to be text or voice
	int position;
	BOOL is_private;
	Overwrite permission_overwrites;
	char* topic;
	snowflake last_message_id;
	int bitrate;
	int user_limit;
} Channel;

typedef struct{
  User inviter;
  int uses;
  int max_uses;
  int max_age;
  BOOL temporary;
  char *created_at; //convert this
  BOOL revoked;
} Invite_Metadata;

typedef struct{
  snowflake id;
  char *name;
  char *splash;
  char *icon;
} Invite_Guild;

typedef struct{ //thanks boysanic
  snowflake channel_id;
  char *name;
  char *type;
} Invite_Channel;

typedef struct{
	snowflake id;
	BOOL is_private;
	User recipient;
	snowflake last_message_id;
} DMChannel;

typedef struct{
	snowflake id;
	snowflake channel_id;
	User author;
	char* content;
	char* timestamp;
	char* edited_timestamp;
	int tts;
	int mention_everyone;
	User *mentions; //TODO you have to malloc this stuff. Its an array of users
	Role *roles; //TODO same as before
	Attachment *attatchments; //TODO same as before yet again
	Embed *embeds; //TODo same
	Reaction *reactions; //same again
	snowflake nonce;
	int pinned;
	char* webhook_id;
} Message;

typedef struct{
  snowflake id;
  char *name;
  char *icon;
  char *splash;
  snowflake owner_id;
  char *region;
  snowflake afk_channel_id;
  int afk_timeout;
  BOOL embed_enabled;
  snowflake embed_channel_id;
  int verification_level;
  int default_message_notifications;
  Role *roles;
  Emoji *emojis;
  //Features *features; //the fuck is this?
  int mfa_level;
  char *joined_at;//this is a datetime. Format this
  BOOL large;
  BOOL unavailable;
  int member_count;
  Voice *voice_states;//array of voice states
  Member *members;//array
  Channel *channels;
  Presence *presences;
} Guild;

typedef struct{
  char *code;
  Guild guild;
  Channel channel;
} Invite;
