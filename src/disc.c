#include <stdio.h>
#include <stdlib.h>

#include "../include/disc.h"

sessionID sessionIDNum = 0; //iterate the session id with every session. TODO REPLACE THIS WITH AN ARRAY OF SESSIONS SO THEY CAN BE FREED
//TODO TODO TODO Add error callback


int DISC_CreateSession(Session *session, Callbacks *callbacks, char* token, int isBot, int logType){ //takes the token, if it's a bot, and a pointer to the ID and returns error if it didnt work


  if(sessionIDNum == 0){
    //setup sockets
    DISC_socket_setup();
  }

  //set up the session
  session->logtype = (BOOL)logType;
  session->token = token;
  if(isBot){
    session->clientType = isBot;
  }

  //give the callback struct to gateway



  printf("Successfully created session with: Token: %s and it is a %s", token, isBot ? "bot\n" : "user\n");

  /*if(isBot){
    printf("bot\n");
  } else{
    printf("user\n");
  }*/

  //connect to the gateway

  sessionIDNum++;
  DISC_AddError(DISC_RETURN_SUCCESS);
  return DISC_RETURN_SUCCESS;
}

int DISC_DestroySession(Session *session){//uses the ID to disconnect and kill the session cleanly
  if(sessionIDNum == 1){
    DISC_socket_exit();
  }

  printf("Destroyed session successfully\n");

  DISC_AddError(DISC_RETURN_SUCCESS);
  return DISC_RETURN_SUCCESS;
}

int DISC_HandleEvents(Session *session, unsigned int numOfSessions){//you can have an array of sessions
  printf("Starting The event handler\n");



  DISC_AddError(DISC_RETURN_SUCCESS);
  return DISC_RETURN_SUCCESS;
}
