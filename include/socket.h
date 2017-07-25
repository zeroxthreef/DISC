#include <openssl/ssl.h>
enum DISC_SSL_TYPE{
  DISC_NO_SSL,
  DISC_USE_SSL,
};

int DISC_socket_httpGET(char *URL, unsigned char **data, unsigned long *dataLen, BOOL isSSL, char *headers);

int DISC_socket_httpPOST(char *URL, unsigned char *data, unsigned long dataLen, BOOL isSSL, char *headers);

int DISC_socket_httpPATCH(char *URL, unsigned char *data, unsigned long dataLen, BOOL isSSL, char *headers);

int DISC_socket_httpPUT(char *URL, unsigned char *data, unsigned long dataLen, BOOL isSSL, char *headers);

int DISC_socket_httpDELETE(char *URL, BOOL isSSL, char *headers);

#ifdef _WIN32
int DISC_socket_Connect(char *addressVar, SOCKET *socketType, SSL_CTX **sslctx, BOOL isSSL);
#else
int DISC_socket_Connect(char *addressVar, int *socketType, SSL_CTX **sslctx, BOOL isSSL);
#endif

#ifdef _WIN32
int DISC_socket_Disconnect(SOCKET socketType, SSL_CTX *sslctx);
#else
int DISC_socket_Disconnect(int socketType, SSL_CTX *sslctx);
#endif

#ifdef _WIN32
int DISC_socket_Read(SOCKET socketType, unsigned char **bufferPtr, unsigned long *bufferSizePtr, SSL_CTX *sslctx);
#else
int DISC_socket_Read(int socketType, unsigned char **bufferPtr, unsigned long *bufferSizePtr, SSL_CTX *sslctx);
#endif

#ifdef _WIN32
int DISC_socket_write(SOCKET socketType, unsigned char *bufferPtr, unsigned long bufferSize, SSL_CTX *sslctx);
#else
int DISC_socket_write(int socketType, unsigned char *bufferPtr, unsigned long bufferSize, SSL_CTX *sslctx);
#endif

int DISC_socket_setup();

int DISC_socket_exit();
