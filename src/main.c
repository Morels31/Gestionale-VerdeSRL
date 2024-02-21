#include "common.h"
#include "main.h"
#include "utility.h"
#include "db_utility.h"
#include "shells/magazzino.h"



/*
 *  L'idea delle import è quella di creare una cartella etc con dentro file diversi,
 *  ognuno con dentro una sla riga che rappresenta il valore della variabile corrispettiva.
 *
 */


void dumpResult(MYSQL *conn){
	
	MYSQL_RES *result;
	result = mysql_store_result(conn);
	if(!result) mysqlError(conn, "mysql_store_result() failed");
	
	unsigned nFields;
	nFields = mysql_field_count(conn);
	if(nFields<=0) return;

	MYSQL_ROW row;
	MYSQL_FIELD *field;

	while( (row = mysql_fetch_row(result)) ){
		printf("\n|");
		for(int i=0; i<nFields; i++){
			field = mysql_fetch_field_direct(result, i);
			if(!field) err("mysql_fetch_field_direct() failed");

			printf(" %s |", row[i] ? row[i] : "NULL");
		}
	}

	mysql_free_result(result);
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



MYSQL *dbLogin(dbI *dbInfo){
	MYSQL *conn = mysql_init(NULL);
	if(!conn) err("mysql_init() failed");



	while(1){

		readUsername("Enter Username: ", dbInfo->username);
		readPassword("Enter Password: ", dbInfo->password);

		printf("hostname: %s\nport: %u\ndbName: %s\nusername: %s\npassword: %s\n\n\n", dbInfo->hostname, dbInfo->port, dbInfo->dbName, dbInfo->username, dbInfo->password);


		if(mysql_real_connect(conn, dbInfo->hostname, dbInfo->username, dbInfo->password, dbInfo->dbName, dbInfo->port, NULL, 0)){
			printf("Successfully connected to the database!\n");
			break;
		}
		if(mysql_errno(conn)!=1045) mysqlErr(conn);
		sleep(3);
		printf("Wrong credentials. Retry\n\n");
	
	}
	return conn;
}


void adminShell(MYSQL *conn){
	printf("Logged in as an %s user.\n\n", ADMIN_ROLE);
	unsigned x = 0;

	x = choice("Select beetween those options: ", "Create new user / Modify existing user", "Delete User", "Change my password", NULL);

	
	printf("Chosed: %u\n\n\n\n\n\n", x);
}




int main(int argc, char **argv){
	dbI dbInfo = {0};



#if MYSQL_VERSION_ID < 40101
	err("The installed mysql version is not supported, update it.");
#endif


	importDBInfo(&dbInfo);
	unsigned nOpt = parseCmdLine(argc, argv, &dbInfo);

	MYSQL *conn = dbLogin(&dbInfo);

	char buffer[BUFF_LEN];
	if(nOpt){
		readLine("Do you want to save/overwrite the database connection options? (hostname, port, dbName) [Y/n]: ", "?", BUFF_LEN-1, buffer, NULL);
		if(buffer[0]!='n'){
			exportDBInfo(&dbInfo);
			printf("Options successfully saved.\n");
		}
	}
	
	char role[MAX_ROLE_LEN+1];
	getCurrentRole(conn, role);
	printf("'%s'\n", role); 

/*

	//if(!strcmp(role, ADMIN_ROLE)) adminShell(conn);


	memset(&dbInfo, 0, sizeof(dbI));


	//runQuery(conn, "SELECT CURRENT_ROLE");
	//dumpResult(conn);


	
	char query[] = "CALL editGiacenza( ? , ? , ? )";


	MYSQL_STMT *stmt;
	if(!(stmt = mysql_stmt_init(conn))) err("mysql_stmt_init() failed");

	if(mysql_stmt_prepare(stmt, query, strlen(query))) stmtError(stmt, "mysql_stmt_prepare() failed");

//has to be afer the mysql_stmt_prepare()
	unsigned long nParams = mysql_stmt_param_count(stmt);
	printf("nParams = %lu\n", nParams);
	if(nParams != (unsigned long) 3) err("Wrong number of parameters given to a prepared statement");



	MYSQL_BIND inBind[3];
	memset(inBind, 0, sizeof(inBind));


	char string1[] = "Basilicus";
	char string2[] = "";
	int x = 10;

	inBind[0].buffer_type = MYSQL_TYPE_STRING;
	inBind[0].buffer = string1; 
	inBind[0].buffer_length = strlen(string1);

	inBind[1].buffer_type = MYSQL_TYPE_STRING;
	inBind[1].buffer = string2; 
	inBind[1].buffer_length = strlen(string2);

	inBind[2].buffer_type = MYSQL_TYPE_LONG;
	inBind[2].buffer = &x; 
	inBind[2].buffer_length = sizeof(x);


	if(mysql_stmt_bind_param(stmt, inBind)) stmtError(stmt, "mysql_stmt_bind_param() failed");


	if(mysql_stmt_execute(stmt)) stmtError(stmt, "mysql_stmt_execute() failed");

	x = 100;

	if(mysql_stmt_execute(stmt)) stmtError(stmt, "mysql_stmt_execute() failed");

//	unsigned long affectedRows = mysql_stmt_affected_rows(stmt);
//	if(affectedRows != (unsigned long) -1) printf("Affected rows: %lu\n", affectedRows);
//FUNZIONA SOLO PER LE INSERT ETC





	fflush(stdout);
//	exit(0);

	MYSQL_RES *resMetadata = mysql_stmt_result_metadata(stmt);



	MYSQL_BIND outBind[1];

	char username[1][MAX_USERNAME_LEN+1];
	
	outBind[0].buffer_type = MYSQL_TYPE_STRING;
	outBind[0].buffer = (char *)username;
	outBind[0].buffer_length = MAX_USERNAME_LEN;

	int status;
	while(1){
		status = mysql_stmt_fetch(stmt);
		printf("mysql_stmt_fetch() returned: %d\n", status);
		if(status == MYSQL_NO_DATA || status == 1) break;



	}

	if(mysql_stmt_close(stmt)) stmtError(stmt, "mysql_stmt_close() failed");


*/





	//CHECK mariadb_reconnect() (specific to mariadb) and mysql_ping() (more general)
	//MYSQL_OPT_RECONNECT is deprecated so maybe mysql_ping, and if errors out, connect manually




	shellMagazzino(conn);




	mysql_close(conn);

	fflush(stdout);
	exit(0);
}
