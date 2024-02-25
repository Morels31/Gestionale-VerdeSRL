#pragma once

#include <unistd.h>
#include <termios.h>
#include <poll.h>
#include <ctype.h>
#include <stdarg.h>
#include <limits.h>


void clearStdin(void);
char *readLine(char *askStr, char *errStr, int maxLen, char *optionalDest, size_t *optionalTotChars);
unsigned multipleChoice(char *askStr, ...);
void readUsername(char *askStr, char *dest);
void readPassword(char *askStr, char *dest);
void readPKSpecie(char *nomeLatino, char *colore, size_t *sizeNomeLatino, size_t *sizeColore);
long long readInt(char *askStr, long long min, long long max);
int choice(char *askStr, char *choice1, char *choice2);
int checkGenericString(char *str, const char *charset, size_t maxSize);
int checkUsername(char *username);
int checkPassword(char *psw);
int checkHostname(char *hostname);
int checkDBName(char *dbName);
int checkPort(unsigned port);
unsigned parseCmdLine(int argc, char **argv, dbI *dbInfo);
