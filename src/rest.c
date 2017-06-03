#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/types.h"
#include "../include/rest.h"
#include "../include/socket.h"


int DISC_REST_GetChannel(Channel *channel, snowflake channelID){//TODO TODO make a DM channel variant
  int error = 0;
  unsigned char *response = NULL;
  unsigned long responseLength;

  char *url = NULL;

  asprintf(url, "%s/%lu", DISCORD_REST_BASE_URL, channelID);


  if(DISC_socket_httpGET(url, response, &responseLength, 1)){
    error++;//temporary set this when I finish errors and stuff;
  }
  //verify that it is a channel object, not a DM channel object



  return error;
}


int DISC_REST_ModifyChannel(snowflake channelID, char *name, int position, char *topic, int bitrate, int user_limit){//TODO fire a CHANNEL_UPDATE event in the gateway. Check for manage_guild permissions
  int error = 0;



  return error;
}

int DISC_REST_DeleteChannel(snowflake channelID){
  int error = 0;


  return error;
}

int DISC_REST_GetChannelMessages(snowflake channelID, snowflake around, snowflake before, snowflake after, int limit){
  int error = 0;


  return error;
}

int DISC_REST_GetChannelMessage(Message *message, snowflake channelID, snowflake messageID){
  int error = 0;


  return error;
}

int DISC_REST_CreateMessage(snowflake channelID, char *content, snowflake nonce, BOOL tts, unsigned char *data, Embed embed){//TODO file in ram or from disk?
  int error = 0;

  if(strlen(content) > 2000){
    //return size error
  }


  return error;
}

int DISC_REST_CreateReaction(snowflake channelID, snowflake messageID, Emoji emoji){
  int error = 0;


  return error;
}

int DISC_REST_DeleteReaction(snowflake channelID, snowflake messageID, Emoji emoji){
  int error = 0;


  return error;
}

int DISC_REST_DeleteUserReaction(snowflake channelID, snowflake messageID, snowflake userID, Emoji emoji){
  int error = 0;


  return error;
}

int DISC_REST_GetReactions(User *users, snowflake channelID, snowflake messageID, Emoji emoji){
  int error = 0;


  return error;
}

int DISC_REST_DeleteAllReactions(snowflake channelID, snowflake messageID){
  int error = 0;


  return error;
}

int DISC_REST_EditMessage(snowflake channelID, snowflake messageID, char *content, Embed embed){
  int error = 0;

  if(strlen(content) > 2000){
    //return size error
  }


  return error;
}

int DISC_REST_DeleteMessage(snowflake channelID, snowflake messageID){
  int error = 0;


  return error;
}

int DISC_REST_BulkDeleteMessages(snowflake channelID, snowflake *messageIDs){
  int error = 0;


  return error;
}

int DISC_REST_EditChannelPermissions(snowflake channelID, snowflake overwriteID, int allow, int deny, char *type){
  int error = 0;


  return error;
}

int DISC_REST_GetChannelInvites(snowflake channelID, Invite *invite){
  int error = 0;


  return error;
}

int DISC_REST_CreateChannelInvite(snowflake channelID, Invite *invite, int max_age, int max_uses, BOOL temporary, BOOL unique){
  int error = 0;

  return error;
}

int DISC_REST_DeleteChannelPermission(snowflake channelID, snowflake overwriteID){
  int error = 0;


  return error;
}
int DISC_REST_TriggerTypingIndicator(snowflake channelID){
  int error = 0;


  return error;
}

int DISC_REST_GetPinnedMessages(snowflake channelID, Message *messages){
  int error = 0;

  return error;
}

int DISC_REST_AddPinnedChannelMessage(snowflake channelID, snowflake messageID){
  int error = 0;

  return error;
}

int DISC_REST_DeletePinnedChannelMessage(snowflake channelID, snowflake messageID){
  int error = 0;


  return error;
}

int DISC_REST_GroupDMAddRecipient(snowflake channelID, snowflake userID, char *access_token, char *nick){
  int error = 0;


  return error;
}

int DISC_REST_GroupDMRemoveRecipient(snowflake channelID, snowflake userID){
  int error = 0;

  return error;
}
