#pragma once

#include <mysql.h>



int execStmt(MYSQL_STMT *stmt, MYSQL_BIND *inParams, MYSQL_BIND *outParams, int (*postExecFunc)(MYSQL_STMT *, MYSQL_BIND *));
int saveOutput(MYSQL_STMT *stmt, MYSQL_BIND *ignored);
void printBindedVar(MYSQL_BIND *bind, int padding);
void printDashes(MYSQL_RES *meta);
void printResultSetHeader(MYSQL_RES *meta);
int printResultSet(MYSQL_STMT *stmt, MYSQL_BIND *outParams);
