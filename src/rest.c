#include <stdio.h>
#include <stdlib.h>

#include "../include/types.h"
#include "../include/rest.h"
#include "../include/socket.h"


int DISC_REST_GetChannel(Channel *channel, snowflake channelId){
  int error = 0;
  unsigned char *response = NULL;
  unsigned long responseLength;

  char *url = NULL;

  asprintf(url, "%s%lu", DISCORD_REST_BASE_URL, channelId);


  if(DISC_socket_httpGET(url, response, &responseLength, 1)){
    error++;//temporary set this when I finish errors and stuff;
  }




  return error;
}


int DISC_REST_ModifyChannel(Channel *channel, snowflake channelId){
  int error = 0;



  return error;
}
