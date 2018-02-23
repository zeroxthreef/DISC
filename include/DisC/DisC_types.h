#ifndef TYPES_H__
#define TYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>

#define DISC_RECONNECT_DELAY 10//number of seconds to wait before reconnecting
#define DISC_MAX_RECONNECTS -1//-1 for infinite, but any other number makes it give up
#define DISC_MAX_TIMEOUT 200
#define DISC_SNOWFLAKE_ALLOCSIZE 19

//typedef uint64_t DisC_snowflake_t;
typedef char* DisC_snowflake_t;
typedef uint8_t DisC_BOOL_t;
typedef uint32_t DisC_sessionID_t; //no need for anything larger. No way more than uint sessions will be used ever


enum DisC_BOOL_enum
{
  DISC_FALSE,
  DISC_TRUE
};


enum DisC_accountTypes
{
  DISC_ACCOUNT_TYPE_USER,
  DISC_ACCOUNT_TYPE_BOT
};


typedef struct
{
  char *token;
  DisC_BOOL_t clientType;
  short logLevel;
  const char *internalName;//used for identifying what session is which in logs. Just for fun
  const char *logFileLocation;
  //lib handles the rest of these
  DisC_sessionID_t DONOTSET_id;
  u_int64_t DONOTSET_lastGWTick;
  u_int64_t DONOTSET_lastRESTTick;
  u_int64_t DONOTSET_lastHeartbeatTick;
  u_int32_t DONOTSET_lastSeqNum;//needed for the sequence num in heartbeats
  unsigned short DONOTSET_currentError;
  int DONOTSET_heartbeat_interval;
  SSL_CTX *DONOTSET_rest_ctx;
  BIO *DONOTSET_rest_bio;
  SSL_CTX *DONOTSET_gateway_ctx;
  BIO *DONOTSET_gateway_bio;
  //DisC_callbacks_t *DONOTSET_callbacks; make it store it differently
} DisC_session_t;




typedef void (*DisC_callback_t)(DisC_session_t *session, void *object, unsigned int type);
//typedef void (*DisC_callback_t)(void *object, unsigned int type);



typedef struct
{
  DisC_callback_t ready;
  DisC_callback_t resumed;
  DisC_callback_t channel_create;
  DisC_callback_t channel_update;
  DisC_callback_t channel_delete;
  DisC_callback_t guild_create;
  DisC_callback_t guild_update;
  DisC_callback_t guild_delete;
  DisC_callback_t guild_ban_add;
  DisC_callback_t guild_ban_remove;
  DisC_callback_t guild_emoji_update;
  DisC_callback_t guild_integrations_update;
  DisC_callback_t guild_member_add;
  DisC_callback_t guild_member_remove;
  DisC_callback_t guild_member_update;
  DisC_callback_t guild_members_chunk;
  DisC_callback_t guild_role_create;
  DisC_callback_t guild_role_update;
  DisC_callback_t guild_role_delete;
  DisC_callback_t message_create;
  DisC_callback_t message_update;
  DisC_callback_t message_delete;
  DisC_callback_t message_delete_bulk;
  DisC_callback_t presence_update;
  DisC_callback_t game_object;//this is an event?
  DisC_callback_t typing_start;
  DisC_callback_t user_update;
  DisC_callback_t voice_state_update;
  DisC_callback_t voice_server_update;
} DisC_callbacks_t;

//discord gateway objects
enum DisC_event_data_type
{
  DISC_EVENTDATA_TYPE_OBJECT,
  DISC_EVENTDATA_TYPE_INTEGER,
  DISC_EVENTDATA_TYPE_BOOL
};

enum DisC_gateway_opcode
{
  DISC_OP_DISPATCH,
  DISC_OP_HEARTBEAT,
  DISC_OP_IDENTIFY,
  DISC_OP_STATUS_UPDATE,
  DISC_OP_VOICE_STATUS_UPDATE,
  DISC_OP_VOICE_SERVER_PING,
  DISC_OP_RESUME,
  DISC_OP_RECONNECT,
  DISC_OP_REQUEST_GUILD_MEMBERS,
  DISC_OP_INVALID_SESSION,
  DISC_OP_HELLO,
  DISC_OP_HEARTBEAT_ACK
};

enum DisC_event_status
{
  DISC_STATUS_ONLINE,
  DISC_STATUS_DO_NOT_DISTURB,
  DISC_STATUS_IDLE,
  DISC_STATUS_INVISIBLE,
  DISC_STATUS_OFFLINE
};

enum DisC_event_activity
{
  DISC_ACTIVITY_PLAYING,
  DISC_ACTIVITY_STREAMING,
  DISC_ACTIVITY_LISTENING,
  DISC_ACTIVITY_WATCHING,
  DISC_ACTIVITY_NOTHING
};

typedef struct
{
  int op;
  int dataType;//"(object, integer, bool)"
  char *d;//parse json later
  int s;//"sequence number, used for resuming sessions and heartbeats" OPCODE 0
  char *t;//"the event name for this payload" OPCODE 0
} DisC_gateway_payload_t;


//discord REST objects------------------------------------------

enum DisC_messageType
{
  DISC_MESSAGETYPE_DEFAULT,
  DISC_MESSAGETYPE_RECIPIENT_ADD,
  DISC_MESSAGETYPE_RECIPIENT_REMOVE,
  DISC_MESSAGETYPE_CALL,
  DISC_MESSAGETYPE_CHANNEL_NAME_CHANGE,
  DISC_MESSAGETYPE_CHANNEL_ICON_CHANGE,
  DISC_MESSAGETYPE_CHANNEL_PINNED_MESSAGE,
  DISC_MESSAGETYPE_GUILD_MEMBER_JOIN
};

typedef struct
{
	DisC_snowflake_t id;
	char* type;
	int allow;
	int deny;
} DisC_overwrite_t;

typedef struct
{
  DisC_snowflake_t id;
  char *username;
  char *discriminator;
  char *avatar;
  DisC_BOOL_t bot;
  DisC_BOOL_t mfa_enabled;
  DisC_BOOL_t verified;
  char *email;
} DisC_user_t;

typedef struct
{
  DisC_snowflake_t id;
  char *name;
  int color;
  DisC_BOOL_t hoist;
  int position;
  int permissions;
  DisC_BOOL_t managed;
  DisC_BOOL_t mentionable;
} DisC_role_t;

typedef struct
{
	char* url;
	char* proxy_url;
	int height;
	int width;
} DisC_embed_thumbnail_t;

typedef struct
{
	char* url;
	int height;
	int width;
} DisC_embed_video_t;

typedef struct
{
	char* url;
	char* proxy_url;
	int height;
	int width;
} DisC_embed_image_t;

typedef struct
{
	char* name;
	char* url;
} DisC_embed_provider_t;

typedef struct
{
	char* name;
	char* url;
	char* icon_url;
	char* proxy_icon_url;
} DisC_embed_author_t;

typedef struct
{
	char* text;
	char* icon_url;
	char* proxy_icon_url;
} DisC_embed_footer_t;

typedef struct
{
	DisC_snowflake_t id;
	char* filename;
	int size;
	char* url;
	char* proxy_url;
	int height;
	int width;
} DisC_attachment_t;

typedef struct
{
	DisC_snowflake_t id;
	char* name;
} DisC_emoji_t;

typedef struct
{
	int count;
	int me;
	DisC_emoji_t emoji;
} DisC_reaction_t;

typedef struct
{
	char* name;
	char* value;
	int isInline;
} DisC_embed_field_t;

typedef struct
{
	char* title;
	char* type;
	char* description;
	char* url;
	char* timestamp;//lookup this format so it can be parsed
	int color;
	DisC_embed_footer_t footer;
	DisC_embed_image_t image;
	DisC_embed_thumbnail_t thumbnail;
	DisC_embed_video_t video;
	DisC_embed_provider_t provider;
	DisC_embed_author_t author;
  unsigned long fieldCount;
	DisC_embed_field_t *fields; //TODO this is an array. needs to be allocated and freed
} DisC_embed_t;

typedef struct
{
  DisC_user_t user;
  char *nick;
  unsigned long roleCount;
  DisC_role_t *roles;
  char *joined_at;//parse this later. Its a datetime
  DisC_BOOL_t deaf;
  DisC_BOOL_t mute;
} DisC_member_t;

typedef struct
{
  DisC_snowflake_t id;
  char *name;
} DisC_account_t;

typedef struct
{
  char *name;
  int type;
  char *url;
} DisC_game_t;

typedef struct
{
  DisC_user_t user;
  unsigned long roleCount;
  DisC_snowflake_t *roles;
  DisC_game_t game;
  DisC_snowflake_t guild_id;
  char *status;
} DisC_presence_t;

typedef struct
{
  DisC_snowflake_t id;
  char *name;
  char *type;
  DisC_BOOL_t enabled;
  DisC_BOOL_t syncing;
  DisC_snowflake_t role_id;
  int expire_behavior;
  int expire_grace_period;
  DisC_user_t user;
  DisC_account_t account;
  char *synced_at; //format this later
} DisC_integration_t;

typedef struct
{
  DisC_snowflake_t guild_id;
  DisC_snowflake_t channel_id;
  DisC_snowflake_t user_id;
  char *sssion_id;
  DisC_BOOL_t deaf;
  DisC_BOOL_t mute;
  DisC_BOOL_t self_deaf;
  DisC_BOOL_t self_mute;
  DisC_BOOL_t suppress;
} DisC_voice_t;

typedef struct
{
	DisC_snowflake_t id;
	DisC_snowflake_t guild_id;
	char* name;
	int type;//going to be text or voice
	int position;
	DisC_BOOL_t isNsfw;
  unsigned long permOverwritesCount;
	DisC_overwrite_t *permission_overwrites;
	char* topic;
  unsigned long recipientCount;
  DisC_user_t *recipients;
  char *icon;
  DisC_snowflake_t owner_id;
  DisC_snowflake_t application_id;
  DisC_snowflake_t parent_id;
	DisC_snowflake_t last_message_id;
  char *last_pin_timestamp;
	int bitrate;
	int user_limit;
} DisC_channel_t;

typedef struct
{
  DisC_user_t inviter;
  int uses;
  int max_uses;
  int max_age;
  DisC_BOOL_t temporary;
  char *created_at; //convert this
  DisC_BOOL_t revoked;
} DisC_invite_metadata_t;

typedef struct
{
  DisC_snowflake_t id;
  char *name;
  char *splash;
  char *icon;
} DisC_invite_guild_t;

typedef struct//thanks boysanic
{
  DisC_snowflake_t channel_id;
  char *name;
  char *type;
} DisC_invite_channel_t;

typedef struct
{
	DisC_snowflake_t id;
	DisC_BOOL_t is_private;
	DisC_user_t recipient;
	DisC_snowflake_t last_message_id;
} DisC_dm_channel_t;

typedef struct
{
	DisC_snowflake_t id;
	DisC_snowflake_t channel_id;
	DisC_user_t author;
	char *content;
	char *timestamp;
	char *edited_timestamp;
	int tts;
	int mention_everyone;
  unsigned long mentionCount;
	DisC_user_t *mentions; //TODO you have to malloc this stuff. Its an array of users
  unsigned long roleCount;
	DisC_snowflake_t *roles; //TODO same as before
  unsigned long attachmentCount;
	DisC_attachment_t *attatchments; //TODO same as before yet again
  unsigned long embedCount;
	DisC_embed_t *embeds; //TODO same
  unsigned long reactionCount;
	DisC_reaction_t *reactions; //same again
	DisC_snowflake_t nonce;
	int pinned;
	char* webhook_id;
  int type;
} DisC_message_t;

typedef struct
{
  DisC_snowflake_t id;
  char *name;
  char *icon;
  char *splash;
  DisC_snowflake_t owner_id;
  char *region;
  DisC_snowflake_t afk_channel_id;
  int afk_timeout;
  DisC_BOOL_t embed_enabled;
  DisC_snowflake_t embed_channel_id;
  int verification_level;
  int default_message_notifications;
  unsigned long roleCount;
  DisC_role_t *roles;
  unsigned long emojiCount;
  DisC_emoji_t *emojis;
  unsigned long featureCount;
  //Features *features; //????
  int mfa_level;
  char *joined_at;//this is a datetime. Format this
  DisC_BOOL_t large;
  DisC_BOOL_t unavailable;
  int member_count;
  unsigned long voiceStateCount;
  DisC_voice_t *voice_states;//array of voice states
  unsigned long memberCount;
  DisC_member_t *members;//array
  unsigned long channelCount;
  DisC_channel_t *channels;
  unsigned long precenceCount;
  DisC_presence_t *presences;
} DisC_guild_t;

typedef struct
{
  char *code;
  DisC_guild_t guild;
  DisC_channel_t channel;
} DisC_invite_t;

//discord gateway events----------------------------------------

typedef struct
{
  DisC_BOOL_t isSince;
  int since;//only read if using since
  DisC_game_t game;
  short status;
  DisC_BOOL_t afk;
} DisC_gateway_status_update_t;

#ifdef __cplusplus
}
#endif

#endif /* TYPES_H__*/
