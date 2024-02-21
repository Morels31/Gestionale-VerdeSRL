#include "common.h"
#include "db_utility.h"
#include "utility.h"



MYSQL_STMT *initStmt(MYSQL *conn, char *statement){

	MYSQL_STMT *stmt = mysql_stmt_init(conn);
	if (!stmt) err("mysql_stmt_init() failed");

	if(mysql_stmt_prepare(stmt, statement, strlen(statement))) stmtError(stmt, "mysql_stmt_prepare() failed");

	return stmt;
}



MYSQL_BIND getBindParam(enum enum_field_types type, void *buffer, unsigned long len){

	MYSQL_BIND param;
	memset(&param, 0, sizeof(param));

	param.buffer_type = type;
	param.buffer = buffer;
	param.buffer_length = len;

	return param;
}



#include <fcntl.h>
#include <sys/stat.h>
#include <stddef.h>
#include <sys/types.h>



/*
 *  Reads from a file named 'filename',
 *  and writes the result to the char buffer pointed by 'buffer'.
 *  Assumes size of 'buffer' to be at least 'maxLen' + 1
 *  If the content of the file is longer than 'maxLen' chars, the rest will be ignored.
 *
 *    'buffer' = pointer to the already allocated char buffer
 *    'maxLen' = maximum number of character to read
 *    'filename' = path of the file to read
 *
 *    returns 0 if no error occoured, or,
 *    returns 1 if file does not exist
 */

int readFromFile(char *buffer, int maxLen, char *filename){
	if(!filename || !buffer) error("NULL argument");
	buffer[0] = '\0';
	
	int fd;
	if((fd = open(filename, O_RDONLY, 0600))==-1){
		if(errno!=ENOENT) error("open() failed");
		return 1;
	}

	ssize_t readed; 
	if((readed = read(fd, buffer, maxLen))<0) error("read() failed");
	buffer[readed && buffer[readed-1]=='\n' ? readed-1 : readed] = '\0';  //removes eventual '\n' last char

	close(fd);
	return 0;
}



/*
 *  Writes to a file named 'filename' the string pointed by 'str'.
 *  If the file already exist, will be overwritten, or else will be created.
 *  If the string is empty, it does nothing.
 *
 *    'str' = pointer to the string
 *    'filename' = path of the file to create
 */

void writeToFile(char *str, char *filename){
	if(!filename || !str) error("NULL argument");
	if(!str[0]) return;

	int fd;
	if((fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0600))==-1) error("open() failed");

	ssize_t writed, toWrite;
	toWrite = strlen(str);
	while(toWrite > 0){
		if((writed = write(fd, str, toWrite))<0) error("write() failed");
		toWrite -= writed;
		str += writed;
	}
	close(fd);
}



/*
 *  Imports database "login" data from file
 *  (hostname, port, db name)
 *
 *    'dbInfo' = pointer to a struct where the info will be imported
 *
 */

void importDBInfo(dbI *dbInfo){
	if(!dbInfo) error("NULL argument");
	int tmp;

	tmp = readFromFile(dbInfo->hostname, MAX_HOSTNAME_LEN, DB_HOSTNAME_FILE);
	if(!tmp && checkHostname(dbInfo->hostname)) err("imported hostname is invalid");

	tmp = readFromFile(dbInfo->dbName, MAX_DB_NAME_LEN, DB_NAME_FILE);
	if(!tmp && checkDBName(dbInfo->dbName)) err("imported database name is invalid");

	char buff[BUFF_LEN];
	tmp = readFromFile(buff, BUFF_LEN-1, DB_PORT_FILE);
	
	errno = 0;
	unsigned long port = strtoul(buff, NULL, 10);
	if(errno) error("strtoul() failed");

	if(!tmp && checkPort(port)) err("imported port number is invalid");
	dbInfo->port = port;
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



void runQuery(MYSQL *conn, char *query){
	/* Qui si potrebbe usare mysql_real_query che e' binary safe */
	if(mysql_query(conn, query))
		mysqlError(conn, "mysql_real_query() failed");
}


