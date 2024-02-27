#include "shells.h"






void shellMagazzino(MYSQL *conn){
	editGiacenzaStruct editGiacenzaS = {0};
	newBuyOrderStruct newBuyOrderS = {0};

	initEditGiacenza(conn, &editGiacenzaS);
	initNewBuyOrder(conn, &newBuyOrderS);


	readPKSpecie(editGiacenzaS.nomeLatino, editGiacenzaS.colore, \
		&editGiacenzaS.inParams[0].buffer_length, &editGiacenzaS.inParams[1].buffer_length);

	editGiacenzaS.diffGiacenza = readInt("Insert how much to modify the stock (with minus or plus signs): ", MIN_MYSQL_INT, MAX_MYSQL_INT);

	EXEC_STMT(&editGiacenzaS);



	printf("Si\n");
	newBuyOrderS.idFornitore = 1;

	//execGenericStmt((void *)&newBuyOrder, &newBuyOrder.outParams[0]);
	EXEC_STMT(&newBuyOrderS);

	mysql_stmt_fetch(newBuyOrderS.stmt);
	printf("%lu\n", newBuyOrderS.outBuyOrderId);

}
