#include "common.h"
#include "main.h"
#include "utility.h"
#include "db_utility.h"
//#include "shells/amministratore.h"
#include "shells/manager.h"
#include "shells/magazzino.h"
//#include "shells/segreteria.h"





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




int main(int argc, char **argv){
	dbI dbInfo = {0};


#if MYSQL_VERSION_ID < 40101
	err("The installed mysql version is not supported, update it.");
#endif

	unsigned nOptNotImported = importDBInfo(&dbInfo);
	unsigned nOptSpecified = parseCmdLine(argc, argv, &dbInfo);

	if(nOptNotImported==3 && !nOptSpecified){
		printf("Warning: no connection options specified, using defaults.\n");
		printf("Execute \"%s -?\" for the help message.\n\n\n", argv[0]);
	}

	MYSQL *conn = dbLogin(&dbInfo);

	char buffer[BUFF_LEN];
	if(nOptSpecified){
		readLine("Do you want to save/overwrite the database connection options? (hostname, port, dbName) [Y/n]: ", "?", BUFF_LEN-1, buffer, NULL);
		if(buffer[0]!='n'){
			exportDBInfo(&dbInfo);
			printf("Options successfully saved.\n");
		}
	}
	
	char role[MAX_ROLE_LEN+1];
	getCurrentRole(conn, role);

	memset(&dbInfo, 0, sizeof(dbI));

	//if(!strcmp(role, "amministratore")) shellAmministrtore(conn);
	if(!strcmp(role, "manager")) shellManager(conn);
	if(!strcmp(role, "magazzino")) shellMagazzino(conn);
	//if(!strcmp(role, "segreteria")) shellSegreteria(conn);

	//Controllare se il ruolo non è null

	mysql_close(conn);

	fflush(stdout);
	exit(0);
}
