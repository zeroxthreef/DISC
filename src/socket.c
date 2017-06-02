#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <event2/event.h>

#ifdef _WIN32

#include <winsock2.h>
#include <ws2tcpip.h>

#else

#include <sys/socket.h>
#include <netinet/in.h>

#endif

#include "../include/types.h"
#include "../include/socket.h"

//yep. Making my own ssl socket thing because I dislike the ones that I found. It needs ssl too.


int DISC_socket_httpGET(char *URL, unsigned char *data, unsigned long *dataLen, BOOL isSSL){
  int error = 0;


  return error;
}

int DISC_socket_httpPOST(char *URL, unsigned char *data, unsigned long dataLen, BOOL isSSL){
  int error = 0;


  return error;
}

int DISC_socket_httpPATCH(char *URL, unsigned char *data, unsigned long dataLen, BOOL isSSL){
  int error = 0;


  return error;
}

int DISC_socket_httpPUT(char *URL, unsigned char *data, unsigned long dataLen, BOOL isSSL){
  int error = 0;


  return error;
}

int DISC_socket_httpDELETE(char *URL, BOOL isSSL){
  int error = 0;


  return error;
}


//platform dependent code below:
//==============================================================================
#ifdef _WIN32

int DISC_socket_Connect(SOCKET socket){
  int error = 0;


  return error;
}

#else

int DISC_socket_Connect(int socketFD){
  int error = 0;


  return error;
}

#endif
//------------------------------------------------------------------------------

//==============================================================================
#ifdef _WIN32

int DISC_socket_Disconnect(SOCKET socket){
  int error = 0;


  return error;
}

#else

int DISC_socket_Disconnect(int socketFD){
  int error = 0;


  return error;
}

#endif
//------------------------------------------------------------------------------

//==============================================================================
#ifdef _WIN32

int DISC_socket_Read(SOCKET socket, unsigned char *bufferPtr, unsigned long *bufferSizePtr){
  int error = 0;


  return error;
}

#else

int DISC_socket_Read(int socketFD, unsigned char *bufferPtr, unsigned long *bufferSizePtr){
  int error = 0;


  return error;
}

#endif
//------------------------------------------------------------------------------


//==============================================================================
#ifdef _WIN32

int DISC_socket_write(SOCKET socket, unsigned char *bufferPtr, unsigned long bufferSize){
  int error = 0;


  return error;
}

#else

int DISC_socket_write(int socketFD, unsigned char *bufferPtr, unsigned long bufferSize){
  int error = 0;


  return error;
}

#endif
//------------------------------------------------------------------------------
