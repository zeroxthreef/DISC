#ifndef UTIL_H__
#define UTIL_H__

#ifdef __cplusplus
extern "C" {
#endif

#define DISC_VER_MAJOR 0
#define DISC_VER_MINOR 0
#define DISC_VER_REVISION 1

enum DisC_logLevel
{
  DISC_LOG_ERROR_ONLY,
  DISC_LOG_VERBOSE,
  DISC_LOG_VERBOSE_AND_PRINT
};

enum DisC_logSeverity
{
  DISC_SEVERITY_NOTIFY,
  DISC_SEVERITY_WARNING,
  DISC_SEVERITY_ERROR
};

short DisC_util_ReadFile(const char *path, unsigned char **data, unsigned long *size);

short DisC_util_WriteFile(const char *path, unsigned char *data, unsigned long *size);

void DisC_Log(short logLevel, const char *logLoc, unsigned short severity, const char *fmt, ...);

char *DisC_strmkdup(const char *str);

short DisC_asprintf(char **string, const char *fmt, ...);

void DisC_Delay(unsigned long milisec);

u_int64_t DisC_GetTick();

//stupid thing copied from stackoverflow. Im only using it to check my websocket stuff. Will delete
void    print_bits(unsigned char octet);//TODO remove this

//===========================================================================================================

short Disc_util_FreeChannel(DisC_session_t *session, DisC_channel_t *channel);

#ifdef __cplusplus
}
#endif

#endif /* UTIL_H__*/
