#pragma once


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>



#define printNow(s) { \
	printf("%s", s); \
	fflush(stdout); \
}

#define err(str) { \
	fprintf(stderr, "ERROR: %s.\n", str); \
	fflush(stderr); \
	exit(1); \
}

#define error(str) { \
	fprintf(stderr, "ERROR: %s. (func: %s() line: %d)\nERRNO (%d): %s\n", str, __func__, __LINE__, errno, strerror(errno)); \
	fflush(stderr); \
	exit(2); \
}

#define mysqlErr(conn){ \
	if(!conn) error("Unknown MYSQL error"); \
	fprintf(stderr, "ERROR: %s.\n", mysql_error(conn)); \
	exit(3); \
}


#define mysqlError(conn, str){ \
	fprintf (stderr, "MYSQL_ERROR: %s.\n", str); \
    if(conn) fprintf (stderr, "Error %u (%s): %s\n", mysql_errno(conn), mysql_sqlstate(conn), mysql_error(conn)); \
	exit(4); \
}

#define stmtErr(stmt, str){ \
	fprintf (stderr, "STMT_ERROR: %s.\n", str); \
	if(stmt){ \
		if(mysql_stmt_sqlstate(stmt)[0]=='4' && mysql_stmt_sqlstate(stmt)[1]=='5'){ \
			fprintf (stderr, "Warning (%s): %s\n\n", mysql_stmt_sqlstate(stmt), mysql_stmt_error(stmt)); \
			return 1; \
		} \
		fprintf (stderr, "Error %u (%s): %s\n", mysql_stmt_errno(stmt), mysql_stmt_sqlstate(stmt), mysql_stmt_error(stmt)); \
	} \
	exit(5); \
}

#define stmtError(stmt, str){ \
	fprintf (stderr, "STMT_ERROR: %s.\n", str); \
	if(stmt) fprintf (stderr, "Error %u (%s): %s\n", mysql_stmt_errno(stmt), mysql_stmt_sqlstate(stmt), mysql_stmt_error(stmt)); \
	exit(5); \
}



#define ETC_FOLDER "etc/"
#define DB_HOSTNAME_FILE ETC_FOLDER "hostname"
#define DB_PORT_FILE ETC_FOLDER "port"
#define DB_NAME_FILE ETC_FOLDER "dbname"


#define DB_NAME_CHARSET "_$"
#define HOSTNAME_CHARSET "-_."
#define USERNAME_CHARSET "-_"
#define PASSWORD_CHARSET "-_.!$<'>/#+-=()[]"
#define HASH_CHARSET "./"

#define MIN_MYSQL_INT (-((long long) 1<<31))
#define MAX_MYSQL_INT (((long long) 1<<31) -1)
#define MAX_MYSQL_UINT (((unsigned long long) 1<<32) -1)

#define MAX_DB_NAME_LEN 64
#define MAX_HOSTNAME_LEN 255
#define MAX_USERNAME_LEN 128
#define MIN_PASSWORD_LEN 7
#define MAX_PASSWORD_LEN 80
#define MAX_ROLE_LEN 25

#define MAX_NOME_LATINO_LEN 100
#define MAX_COLORE_LEN 30
#define MAX_NOME_COMUNE_LEN 50
#define MAX_IVA_LEN 15
#define MAX_CONTATTO_LEN 15
#define MAX_NOME_FORNITORE_LEN 50
#define MAX_GIARD_APPART_LEN 30
#define MAX_VERDE_FIORITA_LEN 15

/* Has to be bigger than the biggest MAX_*_LEN value +1 */
#define BUFF_LEN 1024


#define ADMIN_ROLE "administrator"
#define MANAGER_ROLE "manager"
#define WAREHOUSE_ROLE "warehouse"
