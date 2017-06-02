#include <stdio.h>
#include <stdlib.h>
#include "../include/disc.h"

Session session;

Callbacks callbacks;//make a command callback struct too

void connect(Session *session, void *object){
  printf("Connected\n");
}
//Prototype code that im experimenting with to see how I want to do stuff with the objects
/*
void message(Session *session, void *object){
  Message message = *object;


  DISC_util_IDToText();
  char *string
  asprintf(string, "Message sent by %s in %s\n", message.content, message.author.username);

  printf("%s", string);
  DISC_sendMessage(&session, message.channel_id, string);

}
*/

int main(int argc, char *argv[]){
  callbacks.ready = connect;

  //DISC_InitCommands();
  //DISC_AddCommands();




  DISC_CreateSession(&session, "token goes here :)", DISC_ACCOUNT_TYPE_BOT);


  DISC_HandleEvents(&session);



  DISC_DestroySession(&session);

  return 0;
}
