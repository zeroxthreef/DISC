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


#ifdef _WIN32
int DISC_socket_Connect(SOCKET socket);
#else
int DISC_socket_Connect(int socketFD);
#endif

#ifdef _WIN32
int DISC_socket_Disconnect(SOCKET socket);
#else
int DISC_socket_Disconnect(int socketFD);
#endif

#ifdef _WIN32
int DISC_socket_Read(SOCKET socket, unsigned char *bufferPtr, unsigned long *bufferSizePtr);
#else
int DISC_socket_Read(int socketFD, unsigned char *bufferPtr, unsigned long *bufferSizePtr);
#endif

#ifdef _WIN32
int DISC_socket_write(SOCKET socket, unsigned char *bufferPtr, unsigned long bufferSize);
#else
int DISC_socket_write(int socketFD, unsigned char *bufferPtr, unsigned long bufferSize);
#endif
