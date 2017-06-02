#include "../include/rest.h"
#include "../include/types.h"
Channel DISC_REST_GetChannel(snowflake channelId){
  int error = 0;
  Channel *channel;
  char *url = DISCORD_GATEWAY_BASE_URL + "/channels/" + channelid;
  char *message_fmt = "GET " + *url;
  //Insert web request code here lol I'm bad
  return *channel;
}


int DISC_REST_ModifyChannel(Channel *channel){
  int error = 0;



  return error;
}
