#include "common.h"
#include "statements.h"
#include "utility.h"
#include "db_utility.h"




int execStmt(MYSQL_STMT *stmt, MYSQL_BIND *inParams, MYSQL_BIND *outParams, int (*postExecFunc)(MYSQL_STMT *, MYSQL_BIND *)){

	if(inParams){
		if(mysql_stmt_bind_param(stmt, inParams)) stmtError(stmt, "mysql_stmt_bind_param() failed");
	}

	if(mysql_stmt_execute(stmt)) stmtErr(stmt, "mysql_stmt_execute() failed");

	if(outParams){
		if(mysql_stmt_bind_result(stmt, outParams)) stmtError(stmt, "mysql_stmt_bind_result() failed");
	}

	if(postExecFunc){
		return (*postExecFunc)(stmt, outParams);
	}

	return 0;
}


int saveOutput(MYSQL_STMT *stmt, MYSQL_BIND *ignored){
	if(mysql_stmt_store_result(stmt)) stmtError(stmt, "mysql_stmt_store_result() failed");

	//potrebbero non esserci risultati
	int tmp = mysql_stmt_fetch(stmt);
	if(tmp){
		if(tmp==MYSQL_NO_DATA){
			printf("Received empty result\n\n");
			if(mysql_stmt_reset(stmt)) stmtError(stmt, "mysql_stmt_reset() failed");
			return 1;
		}
		stmtError(stmt, "mysql_stmt_fetch() failed");
	}
	if(mysql_stmt_reset(stmt)) stmtError(stmt, "mysql_stmt_reset() failed");
	return 0;
}

void printBindedVar(MYSQL_BIND *bind, int padding){
	if(bind->buffer_type == MYSQL_TYPE_STRING || bind->buffer_type == MYSQL_TYPE_VAR_STRING){
		printf(" %-*s |", padding, bind->buffer);
	}
	else if(bind->buffer_type == MYSQL_TYPE_TINY){
		if(bind->is_unsigned) printf(" %-*u |", padding, *(unsigned char *) bind->buffer);
		else printf(" %-*d |", padding, *(char *)bind->buffer);
	}
	else if(bind->buffer_type == MYSQL_TYPE_LONG){
		if(bind->is_unsigned) printf(" %-*lu |", padding, *(unsigned long *)bind->buffer);
		else printf(" %-*ld |", padding, *(long *)bind->buffer);
	}
	else{
		printf("ERROR: UNSUPPORTED VAR TYPE");
	}
}



void printDashes(MYSQL_RES *meta){
	MYSQL_FIELD *field;

	putchar('+');
	for(unsigned i=0; i<mysql_num_fields(meta); i++){

		field = mysql_fetch_field_direct(meta, i);
		for(unsigned j=0; j<field->max_length+2; j++) putchar('-');

		putchar('+');
	}
	putchar('\n');
}


/*
 *  Prints the header of the result set
 *  (the name of the columns)
 */

void printResultSetHeader(MYSQL_RES *meta){
	MYSQL_FIELD *field;
	unsigned colLen, i;

	unsigned nFields = mysql_num_fields(meta);
	for(i=0; i<nFields; i++){

		field = mysql_fetch_field_direct(meta, i);
		colLen = strlen(field->name);

		if(colLen < field->max_length) colLen = field->max_length;
		if(colLen < 4 && !IS_NOT_NULL(field->flags)) colLen = 4;

		field->max_length = colLen;
	}

	printDashes(meta);
	putchar('|');

	for(i=0; i<nFields; i++){

		field = mysql_fetch_field_direct(meta, i);
		printf(" %-*s |", (int)field->max_length, field->name);

	}

	putchar('\n');
	printDashes(meta);
}



int printResultSet(MYSQL_STMT *stmt, MYSQL_BIND *outParams){
	if(mysql_stmt_store_result(stmt)) stmtError(stmt, "mysql_stmt_store_result() failed");

	MYSQL_RES *meta = mysql_stmt_result_metadata(stmt);
	if(!meta){
		printf("ERROR: no metadata exists for this prepared statement");
		return 1;
	}

	unsigned nFields = mysql_stmt_field_count(stmt);
	if(!nFields) err("this statement does not produce a result set!");

	printf("\n\n");
	printResultSetHeader(meta);

	MYSQL_FIELD *field;
	while(!mysql_stmt_fetch(stmt)){

		putchar('|');
		for(unsigned i=0; i<nFields; i++){

			field = mysql_fetch_field_direct(meta, i);

			if(outParams[i].is_null_value)
				printf(" %-*s |", (int)field->max_length, "NULL");
			else
				printBindedVar(&outParams[i], (int)field->max_length);
		}
		putchar('\n');
		printDashes(meta);
	}

	mysql_free_result(meta);
	return 0;
}









void initAddSpecie(MYSQL *conn, addSpecieStruct *st){

	st->stmt = initStmt(conn, "CALL addSpecie( ? , ? , ? , ?, ?, ? )");

	st->inParams[0] = getBindParam(MYSQL_TYPE_STRING, st->nomeLatino, 0);
	st->inParams[1] = getBindParam(MYSQL_TYPE_STRING, st->colore, 0);
	st->inParams[2] = getBindParam(MYSQL_TYPE_VAR_STRING, st->nomeComune, 0);
	st->inParams[3] = getBindParam(MYSQL_TYPE_TINY, &st->esotica, sizeof(st->esotica));
	st->inParams[4] = getBindParam(MYSQL_TYPE_TINY, &st->giardino, sizeof(st->giardino));
	st->inParams[5] = getBindParam(MYSQL_TYPE_TINY, &st->fiorita, sizeof(st->fiorita));
}




void initSetPrezzo(MYSQL *conn, setPrezzoStruct *st){

	st->stmt = initStmt(conn, "CALL setPrezzo( ? , ? , ? )");

	st->inParams[0] = getBindParam(MYSQL_TYPE_STRING, st->nomeLatino, 0);
	st->inParams[1] = getBindParam(MYSQL_TYPE_STRING, st->colore, 0);
	st->inParams[2] = getBindParam(MYSQL_TYPE_LONG, &st->newPrezzo, sizeof(st->newPrezzo));
	st->inParams[2].is_unsigned = 1;
}



void initEditGiacenza(MYSQL *conn, editGiacenzaStruct *st){

	st->stmt = initStmt(conn, "CALL editGiacenza( ? , ? , ? )");

	st->inParams[0] = getBindParam(MYSQL_TYPE_STRING, st->nomeLatino, 0);
	st->inParams[1] = getBindParam(MYSQL_TYPE_STRING, st->colore, 0);
	st->inParams[2] = getBindParam(MYSQL_TYPE_LONG, &st->diffGiacenza, sizeof(st->diffGiacenza));
}


void initGetFornitoriForSpecie(MYSQL *conn, getFornitoriForSpecieStruct *st){

	st->stmt = initStmt(conn, "CALL getFornitoriForSpecie( ? , ? )");

	st->inParams[0] = getBindParam(MYSQL_TYPE_STRING, st->nomeLatino, 0);
	st->inParams[1] = getBindParam(MYSQL_TYPE_STRING, st->colore, 0);
	st->outParams[0] = getBindParam(MYSQL_TYPE_LONG, &st->outIdFornitore, sizeof(st->outIdFornitore));
	st->outParams[1] = getBindParam(MYSQL_TYPE_VAR_STRING, st->outNomeFornitore, MAX_NOME_FORNITORE_LEN);
}



void initNewBuyOrder(MYSQL *conn, newBuyOrderStruct *st){

	st->stmt = initStmt(conn, "CALL newBuyOrder( ? , ? )");

	st->inParams[0] = getBindParam(MYSQL_TYPE_LONG, &st->idFornitore, sizeof(st->idFornitore));
	st->outParams[0] = getBindParam(MYSQL_TYPE_LONG, &st->outBuyOrderId, sizeof(st->outBuyOrderId));

	st->inParams[0].is_unsigned = 1;
	st->outParams[0].is_unsigned = 1;

	st->postExecFunc = &saveOutput;
}



void initAddSpecieToBuyOrder(MYSQL *conn, addSpecieToBuyOrderStruct *st){

	st->stmt = initStmt(conn, "CALL addSpecieToBuyOrder( ? , ? , ? , ? )");

	st->inParams[0] = getBindParam(MYSQL_TYPE_STRING, st->nomeLatino, 0);
	st->inParams[1] = getBindParam(MYSQL_TYPE_STRING, st->colore, 0);
	st->inParams[2] = getBindParam(MYSQL_TYPE_LONG, &st->quantita, sizeof(st->quantita));
	st->inParams[3] = getBindParam(MYSQL_TYPE_LONG, &st->buyOrderId, sizeof(st->buyOrderId));

	st->inParams[2].is_unsigned = 1;
	st->inParams[3].is_unsigned = 1;
}



void initNewSellOrder(MYSQL *conn, newSellOrderStruct *st){

	st->stmt = initStmt(conn, "CALL newSellOrder( ? , ? , ? )");

	st->inParams[0] = getBindParam(MYSQL_TYPE_STRING, st->pIVA, 0);
	st->inParams[1] = getBindParam(MYSQL_TYPE_VAR_STRING, st->contatto, 0);
	st->outParams[0] = getBindParam(MYSQL_TYPE_LONG, &st->outSellOrderId, sizeof(st->outSellOrderId));

	st->outParams[0].is_unsigned = 1;

	st->postExecFunc = &saveOutput;
}



void initAddSpecieToSellOrder(MYSQL *conn, addSpecieToSellOrderStruct *st){

	st->stmt = initStmt(conn, "CALL addSpecieToSellOrder( ? , ? , ? , ? )");

	st->inParams[0] = getBindParam(MYSQL_TYPE_STRING, st->nomeLatino, 0);
	st->inParams[1] = getBindParam(MYSQL_TYPE_STRING, st->colore, 0);
	st->inParams[2] = getBindParam(MYSQL_TYPE_LONG, &st->quantita, sizeof(st->quantita));
	st->inParams[3] = getBindParam(MYSQL_TYPE_LONG, &st->sellOrderId, sizeof(st->sellOrderId));

	st->inParams[2].is_unsigned = 1;
	st->inParams[3].is_unsigned = 1;
}



void initGetCostoOrdine(MYSQL *conn, getCostoOrdineStruct *st){

	st->stmt = initStmt(conn, "CALL getCostoOrdine( ? , ? )");

	st->inParams[0] = getBindParam(MYSQL_TYPE_LONG, &st->sellOrderId, sizeof(st->sellOrderId));
	st->outParams[0] = getBindParam(MYSQL_TYPE_LONG, &st->outCost, sizeof(st->outCost));

	st->inParams[0].is_unsigned = 1;
	st->outParams[0].is_unsigned = 1;

	st->postExecFunc = &saveOutput;
}


void initConfirmPayment(MYSQL *conn, confirmPaymentStruct *st){

	st->stmt = initStmt(conn, "CALL confirmPayment( ? )");

	st->inParams[0] = getBindParam(MYSQL_TYPE_LONG, &st->sellOrderId, sizeof(st->sellOrderId));

	st->inParams[0].is_unsigned = 1;
}



void initChangePassword(MYSQL *conn, changePasswordStruct *st){

	st->stmt = initStmt(conn, "SET PASSWORD = PASSWORD('?')");

	st->inParams[0] = getBindParam(MYSQL_TYPE_STRING, st->newPsw, 0);
}


void initAddNewUser(MYSQL *conn, addNewUserStruct *st){

	st->stmt = initStmt(conn, "CREATE OR REPLACE USER ? IDENTIFIED BY ? ; GRANT ? TO ? ; SET DEFAULT ROLE ? FOR ? ");

	st->inParams[0] = getBindParam(MYSQL_TYPE_STRING, st->username, 0);
	st->inParams[1] = getBindParam(MYSQL_TYPE_STRING, st->password, 0);
	st->inParams[2] = getBindParam(MYSQL_TYPE_STRING, st->role, 0);
	st->inParams[3] = getBindParam(MYSQL_TYPE_STRING, st->username, 0);
	st->inParams[2] = getBindParam(MYSQL_TYPE_STRING, st->role, 0);
	st->inParams[3] = getBindParam(MYSQL_TYPE_STRING, st->username, 0);
}


void initDropUser(MYSQL *conn, dropUserStruct *st){

	st->stmt = initStmt(conn, "DROP USER IF EXISTS ?");

	st->inParams[0] = getBindParam(MYSQL_TYPE_STRING, st->username, 0);
}


void initGetSpecie(MYSQL *conn, getSpecieStruct *st){

	st->stmt = initStmt(conn, "SELECT nomeLatino AS a, colore, nomeComune, giacenza, prezzo, esotica, giardAppart, CASE WHEN colore = '' THEN 'verde' ELSE 'fiorita' END AS verdeFiorita FROM Specie");

	st->outParams[0] = getBindParam(MYSQL_TYPE_STRING, st->nomeLatino, MAX_NOME_LATINO_LEN);
	st->outParams[1] = getBindParam(MYSQL_TYPE_STRING, st->colore, MAX_COLORE_LEN);
	st->outParams[2] = getBindParam(MYSQL_TYPE_VAR_STRING, st->nomeComune,MAX_NOME_COMUNE_LEN);
	st->outParams[3] = getBindParam(MYSQL_TYPE_LONG, &st->giacenza, sizeof(st->giacenza));
	st->outParams[4] = getBindParam(MYSQL_TYPE_LONG, &st->prezzo, sizeof(st->prezzo));
	st->outParams[5] = getBindParam(MYSQL_TYPE_TINY, &st->esotica, sizeof(st->esotica));
	st->outParams[6] = getBindParam(MYSQL_TYPE_STRING, st->giardAppart, MAX_GIARD_APPART_LEN);
	st->outParams[7] = getBindParam(MYSQL_TYPE_STRING, st->verdeFiorita, MAX_VERDE_FIORITA_LEN);

	st->outParams[4].is_unsigned = 1;
	
	st->postExecFunc = &printResultSet;
}



