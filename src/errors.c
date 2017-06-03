#include <stdio.h>
#include <stdlib.h>

#include "../include/errors.h"

char *errors[] = {
  "Operation was successfull.",
  "There was a generic error. This is most likely not severe.",
  "The account has insufficient permissins to do this.",
  "The API server returned an error that isn't defined.",
  "Your session has lost connection."
  "The object requested doesnt exist."
};

int currentError = 0;

void DISC_PrintError(){
  printf("%s\n", errors[currentError]);
  currentError = 0;
}

void DISC_AddError(int error){
  currentError = error;
}
