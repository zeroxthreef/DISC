#include <stdio.h>
#include <stdlib.h>

#include "../include/errors.h"

char *errors[] = {
  "Operation was successfull.\0",
  "There was a generic error. This is most likely not severe.\0",
  "The account has insufficient permissions to do this.\0",
  "The API server returned an error that isn't defined.\0",
  "The session has lost connection or not connected.\0"
  "The object requested doesnt exist.\0"
  "Either the message was too large or what was sent exceeded limits for Discord.\0"
  "Not enough ram or memory corruption.\0"
};

int currentError = 0;

void DISC_PrintError(){
  printf("%s\n", errors[currentError]);
  currentError = 0;
}

void DISC_AddError(int error){
  currentError = error;
}
