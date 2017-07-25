#define DISC_VER_MAJOR 0
#define DISC_VER_MINOR 0
#define DISC_VER_REVISION 1

int DISC_util_LoadFile(char *URL, unsigned char *data, unsigned long *size);

int DISC_util_WriteFile(char *URL, unsigned char *data);
