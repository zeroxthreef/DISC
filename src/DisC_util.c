#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include "DisC_types.h"
#include "DisC_errors.h"
#include "DisC_util.h"

short DisC_util_ReadFile(const char *path, unsigned char **data, unsigned long *size)
{




  return DISC_ERROR_NONE;
}

short DisC_util_WriteFile(const char *path, unsigned char *data, unsigned long *size)
{




  return DISC_ERROR_NONE;
}

void DisC_Log(short logLevel, const char *logLoc, unsigned short severity, const char *fmt, ...)
{
  va_list list;
  char *finalString = NULL;
  char *tempString = NULL;
  char tempBuffer[1];
  int size = 0;
  FILE *logFile = NULL;

  va_start(list, fmt);
  size = vsnprintf(tempBuffer, 1, fmt, list);
  va_end(list);
  va_start(list, fmt);
  //printf("%d\n", size);
  if((tempString = malloc(size + 1)) != NULL)
  {
    if(vsnprintf(tempString, size + 1, fmt, list) != -1)
    {
      //printf("%s\n", tempString);
      //write to file and print
      if(logLoc == NULL)//if file doesnt exist, use the default logfile
      {
        logLoc = "log.txt";
      }
      logFile = fopen(logLoc, "a");

      if(logFile == NULL)//create a logfile if it doesnt already exist
      {
        logFile = fopen(logLoc, "w");
        fclose(logFile);
        logFile = fopen(logLoc, "a");
      }
      //time stuff
      time_t t = time(NULL);
    	struct tm *tm = localtime(&t);

    	char* timeStr = asctime(tm);
    	timeStr[strlen(timeStr)-1] = '\0';
      //time stuff

      //if((finalString = calloc((strlen(timeStr) * sizeof(char)) + (strlen(tempString) * sizeof(char)) + 2), sizeof(char)) == NULL)//2 because the 0x00 and space
      if((finalString = calloc(strlen(timeStr) + strlen(tempString) + 2, sizeof(char))) == NULL)//2 because the 0x00 and space
      {
        exit(1);
      }
      //*finalString = 0x00;

      strcat(finalString, timeStr);
      strcat(finalString, " ");
      strcat(finalString, tempString);

      //check severity as well as log level and act accordingly

      switch(logLevel)
      {
        case 0:
          if(severity == 2)
            fprintf(logFile, "%s\n", finalString);
        break;
        case 1:
          fprintf(logFile, "%s\n", finalString);
        break;
        case 2:
          fprintf(logFile, "%s\n", finalString);
          fprintf(stdout, "%s\n", finalString);
        break;
      }

      fclose(logFile);
      free(tempString);
      free(finalString);
      //free(timeStr);
    }
  }
  else
  {
    exit(1);
  }

  va_end(list);

}

char *DisC_strmkdup(const char *str)
{
  if(str != NULL)
  {
    const char *returnStr = malloc((strlen(str) + 1) * sizeof(const char));
    if(returnStr != NULL)
    {
      strcpy(returnStr, str);
    }
      return (char *)returnStr;
  }
  else
  {
    return NULL;
  }
}
//TODO if I need to duplicate intergers too

short DisC_asprintf(char **string, const char *fmt, ...)
{
  va_list list;
  char *tempString = NULL;
  char *oldstring = NULL;
  int size = 0;

  if(*string != NULL)
  {
    //free(*string);
    oldstring = *string;
  }

  va_start(list, fmt);
  size = vsnprintf(tempString, 0, fmt, list);
  va_end(list);
  va_start(list, fmt);

  if((tempString = malloc(size + 1)) != NULL)
  {
    if(vsnprintf(tempString, size + 1, fmt, list) != -1)
    {
      *string = tempString;
      if(oldstring != NULL)
      {
        free(oldstring);
      }
      return size;
    }
    else
    {
      *string = NULL;
      if(oldstring != NULL)
      {
        free(oldstring);
      }
      return -1;
    }
  }
  va_end(list);
}

//cleaners======================================================================

short Disc_util_FreeChannel(DisC_session_t *session, DisC_channel_t *channel)
{

  DisC_AddError(session, DISC_ERROR_NONE);
  return DISC_ERROR_NONE;
}
