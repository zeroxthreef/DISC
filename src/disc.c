#include <stdio.h>
#include <stdlib.h>

#include "../include/disc.h"

sessionID sessionIDNum = 0; //iterate the session id with every session. TODO REPLACE THIS WITH AN ARRAY OF SESSIONS SO THEY CAN BE FREED


int DISC_CreateSession(Session *session, char* token, int isBot, int logType){ //takes the token, if it's a bot, and a pointer to the ID and returns error if it didnt work
  int error = 0;//TODO make an error system to parse errors that this returns


  //set up the session
  session->logtype = (BOOL)logType;
  session->token = token;
  if(isBot){
    session->clientType = isBot;
  }



  printf("Successfully created session with: Token: %s and it is a ", token);

  if(isBot){
    printf("bot\n");
  } else{
    printf("user\n");
  }

  //connect to the gateway



  return error;
}

int DISC_DestroySession(Session *session){//uses the ID to disconnect and kill the session cleanly
  int error = 0;


  printf("Destroyed session successfully\n");
  return error;
}

int DISC_HandleEvents(Session *session, unsigned int numOfSessions){//you can have an array of sessions
  int error = 0;
  printf("Starting The event handler\n");


  return error;
}
