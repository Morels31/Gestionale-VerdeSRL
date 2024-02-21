#pragma once


#include <mysql.h>



typedef struct genericStatementStruct{
		MYSQL_STMT *stmt;
		MYSQL_BIND param[];
} genericStmtStruct;

typedef struct editGiacenzaStatementStruct {
        MYSQL_STMT *stmt;
        MYSQL_BIND param[3];
        char nomeLatino[MAX_NOME_LATINO_LEN+1];
        char colore[MAX_COLORE_LEN+1];
        int diffGiacenza;
} editGiacenzaStruct;




void execGenericStmt(genericStmtStruct *st);
void initEditGiacenza(MYSQL *conn, editGiacenzaStruct *st);
void shellMagazzino(MYSQL *conn);
