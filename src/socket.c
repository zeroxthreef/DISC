#include "../include/socket.h"

//yep. Making my own ssl socket thing because I dislike the ones that I found. It needs ssl too.
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
