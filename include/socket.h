int DISC_socket_httpGET(char *URL, unsigned char *data, unsigned long *dataLen, BOOL isSSL);

int DISC_socket_httpPOST(char *URL, unsigned char *data, unsigned long dataLen, BOOL isSSL);

int DISC_socket_httpPATCH(char *URL, unsigned char *data, unsigned long dataLen, BOOL isSSL);

int DISC_socket_httpPUT(char *URL, unsigned char *data, unsigned long dataLen, BOOL isSSL);

int DISC_socket_httpDELETE(char *URL, BOOL isSSL);

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
