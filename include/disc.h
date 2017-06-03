#include "../include/types.h"
#include "../include/socket.h"
#include "../include/errors.h"


enum DISC_AccountTypes{
	DISC_ACCOUNT_TYPE_USER,
	DISC_ACCOUNT_TYPE_BOT,
};

enum DISC_Logging{
	DISC_LOG_EVERYTHING,
	DISC_LOG_ONLY_IMPORTANT,
	DISC_LOG_NOTHING,
};

int DISC_CreateSession(Session *session, Callbacks *callbacks, char* token, int isBot, int logType);

int DISC_DestroySession(Session *session);

int DISC_HandleEvents(Session *session, unsigned int numOfSessions);
