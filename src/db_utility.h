#pragma once

#include <mysql.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stddef.h>
#include <sys/types.h>



typedef struct dbInformation{
	unsigned port;
	char hostname[MAX_HOSTNAME_LEN+1];
	char dbName[MAX_DB_NAME_LEN+1];
	char username[MAX_USERNAME_LEN+1];
	char password[MAX_PASSWORD_LEN+1];
} dbI;



MYSQL_STMT *initStmt(MYSQL *conn, char *statement);
MYSQL_BIND getBindParam(enum enum_field_types type, void *buffer, unsigned long len);
void getCurrentRole(MYSQL *conn, char *dest);
MYSQL *dbLogin(dbI *dbInfo);
unsigned importDBInfo(dbI *dbInfo);
void exportDBInfo(dbI *dbInfo);
