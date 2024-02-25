#include "common.h"
#include "db_utility.h"
#include "utility.h"



/*
 *  Initializes a statement.
 *
 *    returns the initialized statement
 */

MYSQL_STMT *initStmt(MYSQL *conn, char *statement){

	MYSQL_STMT *stmt = mysql_stmt_init(conn);
	if (!stmt) err("mysql_stmt_init() failed");

	if(mysql_stmt_prepare(stmt, statement, strlen(statement))) stmtError(stmt, "mysql_stmt_prepare() failed");


	unsigned updateLength = 1;
	mysql_stmt_attr_set(stmt, STMT_ATTR_UPDATE_MAX_LENGTH, &updateLength);

	return stmt;
}



/*
 *  This function initializes a MYSQL_BIND struct to 0,
 *  and then fills with the paramaters given in input.
 *
 *    'type' = type of the binded variable.
 *    'buffer' = pointer to where the variable is/will be stored.
 *    'len' = length of the buffer.
 */

MYSQL_BIND getBindParam(enum enum_field_types type, void *buffer, unsigned long len){

	MYSQL_BIND param;
	memset(&param, 0, sizeof(param));

	param.buffer_type = type;
	param.buffer = buffer;
	param.buffer_length = len;

	return param;
}



/*
 *  Queries the database to get the role of the current user.
 *  Saving the result in a char buffer pointed by 'dest'.
 *  (That has to be at least MAX_ROLE_LEN+1)
 *
 *  If the current role is NULL, the first char of 'dest' will be set to '\0'.
 *
 *    'conn' = a db connection handle
 *    'dest' = already allocated char buffer where result will be written
 *
 */

void getCurrentRole(MYSQL *conn, char *dest){

	if(mysql_query(conn, "SELECT CURRENT_ROLE")) mysqlError(conn, "mysql_query() failed");
	
	MYSQL_RES *result;
	result = mysql_use_result(conn);
	if(!result) mysqlError(conn, "mysql_use_result() failed");
	
	unsigned nFields;
	nFields = mysql_field_count(conn);
	if(nFields!=1) err("getCurrentRole() failed");

	MYSQL_ROW row = mysql_fetch_row(result);
	if(!row) mysqlError(conn, "getCurrentRole() failed");

	if(!row[0]) dest[0] = '\0';
	else{
		if(strlen(row[0])>MAX_ROLE_LEN) err("There is a role longer than MAX_ROLE_LEN");
		strcpy(dest, row[0]);
	}
	mysql_free_result(result);
}



/*
 *  Asks the user for username and password,
 *  and then tries to login to the database
 *  with the info provided by the input parameter struct.
 *
 *    'dbInfo' = pointer to a struct where the needed connection info are stored.
 *
 *    returns a valid MYSQL connection handler.
 */

MYSQL *dbLogin(dbI *dbInfo){
	MYSQL *conn = mysql_init(NULL);
	if(!conn) err("mysql_init() failed");

	while(1){

		readUsername("Enter Username: ", dbInfo->username);
		readPassword("Enter Password: ", dbInfo->password);

		if(mysql_real_connect(conn, dbInfo->hostname, dbInfo->username, dbInfo->password, dbInfo->dbName, dbInfo->port, NULL, 0)) break;

		if(mysql_errno(conn)!=1045) mysqlErr(conn);
		sleep(3);
		printf("\nWrong credentials. Retry\n\n");
	
	}
	return conn;
}



/*
 *  Imports database "login" data from file
 *  (hostname, port, db name)
 *
 *    'dbInfo' = pointer to a struct where the info will be imported
 *
 *    returns how many option files are not present
 */

unsigned importDBInfo(dbI *dbInfo){
	if(!dbInfo) error("NULL argument");
	unsigned notReaded = 0;

	notReaded += readFromFile(dbInfo->hostname, MAX_HOSTNAME_LEN, DB_HOSTNAME_FILE);
	if(!notReaded && checkHostname(dbInfo->hostname)) err("imported hostname is invalid");

	notReaded += readFromFile(dbInfo->dbName, MAX_DB_NAME_LEN, DB_NAME_FILE);
	if(!notReaded && checkDBName(dbInfo->dbName)) err("imported database name is invalid");

	char buff[BUFF_LEN];
	notReaded += readFromFile(buff, BUFF_LEN-1, DB_PORT_FILE);
	
	errno = 0;
	unsigned long port = strtoul(buff, NULL, 10);
	if(errno) error("strtoul() failed");

	if(!notReaded && checkPort(port)) err("imported port number is invalid");
	dbInfo->port = port;

	return notReaded;
}



/*
 *  Exports database "login" data to file
 *  (hostname, port, db name)
 *
 *    'dbInfo' = pointer to a struct containing the info to export
 *
 */

void exportDBInfo(dbI *dbInfo){
	if(!dbInfo) error("NULL argument");

	if(mkdir(ETC_FOLDER, 0700)==-1 && errno!=EEXIST) error("mkdir() failed");

	writeToFile(dbInfo->hostname, DB_HOSTNAME_FILE);
	writeToFile(dbInfo->dbName, DB_NAME_FILE);

	if(!dbInfo->port) return;
	char buff[BUFF_LEN];
	sprintf(buff, "%u", dbInfo->port);
	writeToFile(buff, DB_PORT_FILE);
}
