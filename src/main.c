#include "common.h"
#include "db_utility.h"
#include "utility.h"
#include "shells/shells.h"



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

	putchar('\n');
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

	if(!strcmp(role, "amministratore")) shellAmministratore(conn);
	else if(!strcmp(role, "manager")) shellManager(conn);
	else if(!strcmp(role, "magazzino")) shellMagazzino(conn);
	else if(!strcmp(role, "segreteria")) shellSegreteria(conn);
	else printf("You logged in with an user without role.\n");


	mysql_close(conn);

	fflush(stdout);
	exit(0);
}
