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
#include "../include/errors.h"

//yep. Making my own ssl socket thing because I dislike the ones that I found. It needs ssl too.


int DISC_socket_httpGET(char *URL, unsigned char *data, unsigned long *dataLen, BOOL isSSL){



  DISC_AddError(DISC_RETURN_SUCCESS);
  return DISC_RETURN_SUCCESS;
}

int DISC_socket_httpPOST(char *URL, unsigned char *data, unsigned long dataLen, BOOL isSSL){




  DISC_AddError(DISC_RETURN_SUCCESS);
  return DISC_RETURN_SUCCESS;
}

int DISC_socket_httpPATCH(char *URL, unsigned char *data, unsigned long dataLen, BOOL isSSL){




  DISC_AddError(DISC_RETURN_SUCCESS);
  return DISC_RETURN_SUCCESS;
}

int DISC_socket_httpPUT(char *URL, unsigned char *data, unsigned long dataLen, BOOL isSSL){




  DISC_AddError(DISC_RETURN_SUCCESS);
  return DISC_RETURN_SUCCESS;
}

int DISC_socket_httpDELETE(char *URL, BOOL isSSL){




  DISC_AddError(DISC_RETURN_SUCCESS);
  return DISC_RETURN_SUCCESS;
}


//platform dependent code below:
//==============================================================================
#ifdef _WIN32

int DISC_socket_Connect(SOCKET socket){




  DISC_AddError(DISC_RETURN_SUCCESS);
  return DISC_RETURN_SUCCESS;
}

#else

int DISC_socket_Connect(int socketFD){




  DISC_AddError(DISC_RETURN_SUCCESS);
  return DISC_RETURN_SUCCESS;
}

#endif
//------------------------------------------------------------------------------

//==============================================================================
#ifdef _WIN32

int DISC_socket_Disconnect(SOCKET socket){




  DISC_AddError(DISC_RETURN_SUCCESS);
  return DISC_RETURN_SUCCESS;
}

#else

int DISC_socket_Disconnect(int socketFD){




  DISC_AddError(DISC_RETURN_SUCCESS);
  return DISC_RETURN_SUCCESS;
}

#endif
//------------------------------------------------------------------------------

//==============================================================================
#ifdef _WIN32

int DISC_socket_Read(SOCKET socket, unsigned char *bufferPtr, unsigned long *bufferSizePtr){




  DISC_AddError(DISC_RETURN_SUCCESS);
  return DISC_RETURN_SUCCESS;
}

#else

int DISC_socket_Read(int socketFD, unsigned char *bufferPtr, unsigned long *bufferSizePtr){




  DISC_AddError(DISC_RETURN_SUCCESS);
  return DISC_RETURN_SUCCESS;
}

#endif
//------------------------------------------------------------------------------


//==============================================================================
#ifdef _WIN32

int DISC_socket_write(SOCKET socket, unsigned char *bufferPtr, unsigned long bufferSize){




  DISC_AddError(DISC_RETURN_SUCCESS);
  return DISC_RETURN_SUCCESS;
}

#else

int DISC_socket_write(int socketFD, unsigned char *bufferPtr, unsigned long bufferSize){




  DISC_AddError(DISC_RETURN_SUCCESS);
  return DISC_RETURN_SUCCESS;
}

#endif
//------------------------------------------------------------------------------
