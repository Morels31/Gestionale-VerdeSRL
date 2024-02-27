#include "common.h"
#include "db_utility.h"
#include "statements.h"
#include "stmt_utility.h"



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
	st->outParams[0].is_unsigned = 1;

	st->postExecFunc = &printResultSet;
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

	st->stmt = initStmt(conn, "SELECT nomeLatino, colore, nomeComune, giacenza, prezzo, esotica, giardAppart, CASE WHEN colore = '' THEN 'verde' ELSE 'fiorita' END AS verdeFiorita FROM Specie");

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
