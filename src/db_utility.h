#pragma once

#include <mysql.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stddef.h>
#include <sys/types.h>


MYSQL_STMT *initStmt(MYSQL *conn, char *statement);
MYSQL_BIND getBindParam(enum enum_field_types type, void *buffer, unsigned long len);
int readFromFile(char *buffer, int maxLen, char *filename);
void writeToFile(char *str, char *filename);
unsigned importDBInfo(dbI *dbInfo);
void exportDBInfo(dbI *dbInfo);
