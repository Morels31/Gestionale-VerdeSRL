#include "common.h"
#include "statements.h"
#include "utility.h"
#include "db_utility.h"




void execGenericStmt(genericStmtStruct *st){
	if(mysql_stmt_bind_param(st->stmt, st->param)) stmtError(st->stmt, "mysql_stmt_bind_param() failed");

	if(mysql_stmt_execute(st->stmt)) stmtError(st->stmt, "mysql_stmt_execute() failed");

}


void initEditGiacenza(MYSQL *conn, editGiacenzaStruct *st){

	st->stmt = initStmt(conn, "CALL editGiacenza( ? , ? , ? )");

	st->param[0] = getBindParam(MYSQL_TYPE_STRING, st->nomeLatino, MAX_NOME_LATINO_LEN);
	st->param[1] = getBindParam(MYSQL_TYPE_STRING, st->colore, MAX_COLORE_LEN);
	st->param[2] = getBindParam(MYSQL_TYPE_LONG, &st->diffGiacenza, sizeof(st->diffGiacenza));
}

