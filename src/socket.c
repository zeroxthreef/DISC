#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef _WIN32

#include <winsock2.h>
#include <ws2tcpip.h>
#include <sys/types.h>
#include <netinet/in.h>

#else

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>//for gethostbyname and gethostbyaddr
#include <unistd.h>

#endif

#include <openssl/ssl.h>
#include <openssl/err.h>

#include "../include/types.h"
#include "../include/errors.h"
#include "../include/util.h"
#include "../include/socket.h"

const char *defaultHeaders = "User-Agent: DISC (https://github.com/zeroxthreef/DISC, %d.%d.%d)\r\nConnection: close\r\n";

SSL *ssl;
long res = 1;
BIO *web = NULL, *out = NULL;





int DISC_socket_httpGET(char *URL, unsigned char **data, unsigned long *dataLen, BOOL isSSL, char *headers){//send the token too. Needs a special discord case. SEND IT IN THE HEADERS
  int socketType;
  SSL_CTX *sslctx = NULL;;
  const char *httpGET = "GET %s HTTP/1.1\r\n%sHost: %s\r\n%s\r\n";
  char *httpGETString = NULL, *path = NULL, *finalURL = NULL;
  if((path = strstr(URL, "/")) == NULL){
    DISC_AddError(DISC_ERROR_CONNECTION);
    return DISC_ERROR_CONNECTION;
  }
  //path++;

  finalURL = calloc(strlen(URL), sizeof(char));


  char *copy = NULL;
  for(copy = URL; copy != strstr(URL, "/"); copy++){
    strncat(finalURL, copy, 1);
  }


  char *finalDefHeaders = NULL;
  asprintf(&finalDefHeaders, defaultHeaders, DISC_VER_MAJOR, DISC_VER_MINOR, DISC_VER_REVISION);
  if(headers != NULL){
    asprintf(&httpGETString, httpGET, path, finalDefHeaders, finalURL, headers);
  } else{
    asprintf(&httpGETString, httpGET, path, finalDefHeaders, finalURL, "");
  }


  //printf("[%s] [%s] [%s]\n", finalURL, path, httpGETString);


  if(DISC_socket_Connect(finalURL, &socketType, &sslctx, isSSL)){//change URL because it can have locations
    DISC_AddError(DISC_ERROR_CONNECTION);
    return DISC_ERROR_CONNECTION;
  }

  if(DISC_socket_write(socketType, httpGETString, strlen(httpGETString), sslctx)){
    DISC_AddError(DISC_ERROR_CONNECTION);
    return DISC_ERROR_CONNECTION;
  }



  if(DISC_socket_Read(socketType, data, dataLen, sslctx)){
    DISC_AddError(DISC_ERROR_CONNECTION);
    return DISC_ERROR_CONNECTION;
  }


  if(DISC_socket_Disconnect(socketType, sslctx)){
    DISC_AddError(DISC_ERROR_CONNECTION);
    return DISC_ERROR_CONNECTION;
  }




  DISC_AddError(DISC_RETURN_SUCCESS);
  return DISC_RETURN_SUCCESS;
}

int DISC_socket_httpPOST(char *URL, unsigned char *data, unsigned long dataLen, BOOL isSSL, char *headers){




  DISC_AddError(DISC_RETURN_SUCCESS);
  return DISC_RETURN_SUCCESS;
}

int DISC_socket_httpPATCH(char *URL, unsigned char *data, unsigned long dataLen, BOOL isSSL, char *headers){




  DISC_AddError(DISC_RETURN_SUCCESS);
  return DISC_RETURN_SUCCESS;
}

int DISC_socket_httpPUT(char *URL, unsigned char *data, unsigned long dataLen, BOOL isSSL, char *headers){




  DISC_AddError(DISC_RETURN_SUCCESS);
  return DISC_RETURN_SUCCESS;
}

int DISC_socket_httpDELETE(char *URL, BOOL isSSL, char *headers){




  DISC_AddError(DISC_RETURN_SUCCESS);
  return DISC_RETURN_SUCCESS;
}


//platform dependent code below:
//==============================================================================
#ifdef _WIN32

int DISC_socket_Connect(char *addressVar, SOCKET *socketType, SSL_CTX **sslctx, BOOL isSSL){//TODO detect whether IP or domain




  DISC_AddError(DISC_RETURN_SUCCESS);
  return DISC_RETURN_SUCCESS;
}

#else

//linux socket
int DISC_socket_Connect(char *addressVar, int *socketType, SSL_CTX **sslctx, BOOL isSSL){//TODO detect whether IP or domain
  struct sockaddr_in address;
  struct hostent *host = gethostbyname(addressVar);
  *socketType = socket(AF_INET, SOCK_STREAM, 0);

  if(*socket < 0){
    DISC_AddError(DISC_ERROR_CONNECTION);
    return DISC_ERROR_CONNECTION;
  }
  if(host == NULL){
    DISC_AddError(DISC_ERROR_CONNECTION);
    return DISC_ERROR_CONNECTION;
  }

  memset((char *)&address, 0x00, sizeof(address));
  address.sin_family = AF_INET;


  memmove((char *)&address.sin_addr.s_addr, (char *)host->h_addr,host->h_length);
  address.sin_port = htons(80);


  if(connect(*socketType, (struct sockaddr *)&address, sizeof(address)) < 0){
    DISC_AddError(DISC_ERROR_CONNECTION);
    return DISC_ERROR_CONNECTION;
  }

  if(isSSL){
    //*sslctx = SSL_CTX_new(SSLv23_server_method());
    //TLSv1_2_client_method()
    *sslctx = SSL_CTX_new(TLSv1_2_client_method());
    //SSL_CTX_set_options(*sslctx, SSL_OP_NO_SSLv2);
    SSL_CTX_set_options(*sslctx, SSL_OP_SINGLE_DH_USE);
    if ( sslctx == NULL )
    {
        ERR_print_errors_fp(stdout);
    }

    //web = BIO_new_ssl_connect(*sslctx);
    //if(web == NULL){
      //ERR_print_errors_fp(stdout);
    //}

    //res = BIO_set_conn_hostname(web, "discordapp.com:80");
    //BIO_get_ssl(web, &ssl);
    //res = SSL_set_tlsext_host_name(ssl, "discordapp.com");

    ssl = SSL_new(*sslctx);
    SSL_set_fd(ssl, *socketType);
    printf("Encryption %s\n", SSL_get_cipher(ssl));
    //if(SSL_accept(ssl) <= 0){
    if(SSL_connect(ssl) == -1){
      DISC_AddError(DISC_ERROR_CONNECTION);
      return DISC_ERROR_CONNECTION;
    }
  }

  X509 *cert;
  cert = SSL_get_peer_certificate(ssl);
  if(cert != NULL){
    printf("oyoyoyoyoy %s %s\n", X509_NAME_oneline(X509_get_subject_name(cert), 0, 0), X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0));
  } else{
    printf("Goddamnit\n");
  }



  DISC_AddError(DISC_RETURN_SUCCESS);
  return DISC_RETURN_SUCCESS;
}

#endif
//------------------------------------------------------------------------------

//==============================================================================
#ifdef _WIN32

int DISC_socket_Disconnect(SOCKET socketType, SSL_CTX *sslctx){




  DISC_AddError(DISC_RETURN_SUCCESS);
  return DISC_RETURN_SUCCESS;
}

#else

int DISC_socket_Disconnect(int socketType, SSL_CTX *sslctx){

  close(socketType);

  if(sslctx != NULL){
    SSL_CTX_free(sslctx);
  }

  printf("Disconnected\n");

  DISC_AddError(DISC_RETURN_SUCCESS);
  return DISC_RETURN_SUCCESS;
}

#endif
//------------------------------------------------------------------------------

//==============================================================================
#ifdef _WIN32

int DISC_socket_Read(SOCKET socketType, unsigned char **bufferPtr, unsigned long *bufferSizePtr, SSL_CTX *sslctx){




  DISC_AddError(DISC_RETURN_SUCCESS);
  return DISC_RETURN_SUCCESS;
}

#else

int DISC_socket_Read(int socketType, unsigned char **bufferPtr, unsigned long *bufferSizePtr, SSL_CTX *sslctx){
  BOOL readBefore = 0;
  unsigned int bufferSize = 1024, readreturn = 0;
  unsigned char *buffer = NULL, *buffPtr = NULL, *finalBuf = NULL;
  unsigned long bufferTimes = 0;



  if((finalBuf = malloc(bufferSize)) == NULL || (buffer = malloc(bufferSize)) == NULL){
    DISC_AddError(DISC_ERROR_MEMORY);
    return DISC_ERROR_MEMORY;
  }
  printf("Starting read\n");
  //TODO make this less crap
  //while(read(socketType, buffer, bufferSize) > 0){
  if(sslctx != NULL){//if its using SSL
    //readreturn = SSL_read(ssl, buffer, bufferSize);
    while(SSL_read(ssl, buffer, bufferSize) > 0){
      if(readBefore){
        bufferTimes++;
        if((finalBuf = realloc(finalBuf, ((bufferTimes + 1) * bufferSize))) == NULL){
          DISC_AddError(DISC_ERROR_MEMORY);
          return DISC_ERROR_MEMORY;
        }
        memcpy(&finalBuf[(bufferTimes) * bufferSize], buffer, bufferSize);
      } else{
        readBefore++;
        memcpy(finalBuf, buffer, bufferSize);
      }
      printf("%s\n", buffer);
      //memmove(&finalBuf[bufferTimes * bufferSize], buffer, bufferSize);

      //=========================================================
    }
  } else{
    //readreturn = read(socketType, buffer, bufferSize);
    while(read(socketType, buffer, bufferSize) > 0){
      if(readBefore){
        bufferTimes++;
        if((finalBuf = realloc(finalBuf, ((bufferTimes + 1) * bufferSize))) == NULL){
          DISC_AddError(DISC_ERROR_MEMORY);
          return DISC_ERROR_MEMORY;
        }
        //printf("no\n");
        memcpy(&finalBuf[(bufferTimes) * bufferSize], buffer, bufferSize);
      } else{
        //printf("Hey\n");
        readBefore++;
        memcpy(finalBuf, buffer, bufferSize);
        //printf("dfdfdf %s\n", finalBuf);
        //bufferTimes++;
      }

      //printf("%s\n", buffer);
      //printf("gjghj %lu %lu\n", sizeof(finalBuf), bufferTimes * bufferSize);
      //memmove(&finalBuf[bufferTimes * bufferSize], buffer, bufferSize);


      //=========================================================
    }
  }



  *bufferPtr = finalBuf;
  *bufferSizePtr = bufferTimes * bufferSize;

  //printf("read %s %lu\n", *bufferPtr, *bufferSizePtr);


  DISC_AddError(DISC_RETURN_SUCCESS);
  return DISC_RETURN_SUCCESS;
}

#endif
//------------------------------------------------------------------------------


//==============================================================================
#ifdef _WIN32

int DISC_socket_write(SOCKET socketType, unsigned char *bufferPtr, unsigned long bufferSize, SSL_CTX *sslctx){




  DISC_AddError(DISC_RETURN_SUCCESS);
  return DISC_RETURN_SUCCESS;
}

#else

int DISC_socket_write(int socketType, unsigned char *bufferPtr, unsigned long bufferSize, SSL_CTX *sslctx){


  if(sslctx != NULL){//if its using SSL
    printf("ssssslllll\n");
    if(SSL_write(ssl, bufferPtr, bufferSize) < 0){
      DISC_AddError(DISC_ERROR_CONNECTION);
      return DISC_ERROR_CONNECTION;
    }
  } else{
    if(write(socketType, bufferPtr, bufferSize) < 0){
      DISC_AddError(DISC_ERROR_CONNECTION);
      return DISC_ERROR_CONNECTION;
    }
  }



  printf("wrote %s\n", bufferPtr);


  DISC_AddError(DISC_RETURN_SUCCESS);
  return DISC_RETURN_SUCCESS;
}

#endif
//------------------------------------------------------------------------------
int DISC_socket_setup(){
  printf("Setup OpenSSL\n");
  SSL_library_init();
  SSL_load_error_strings();
  OpenSSL_add_all_algorithms();

  DISC_AddError(DISC_RETURN_SUCCESS);
  return DISC_RETURN_SUCCESS;
}

int DISC_socket_exit(){
  SSL_shutdown(ssl);
  EVP_cleanup();
  SSL_free(ssl);

  printf("Shut down OpenSSL\n");

  DISC_AddError(DISC_RETURN_SUCCESS);
  return DISC_RETURN_SUCCESS;
}
