#include "../include/types.h"
#include "../include/socket.h"


enum DISC_AccountTypes{
	DISC_ACCOUNT_TYPE_USER,
	DISC_ACCOUNT_TYPE_BOT,
};

enum DISC_Logging{
	DISC_LOG_EVERYTHING,
	DISC_LOG_ONLY_IMPORTANT,
};

int DISC_CreateSession(Session *session, char* token, int isBot);

int DISC_DestroySession(Session *session);

int DISC_HandleEvents(Session *session);
