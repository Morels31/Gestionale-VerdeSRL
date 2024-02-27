#include "shells.h"






void shellMagazzino(MYSQL *conn){
	editGiacenzaStruct editGiacenzaS = {0};
	newBuyOrderStruct newBuyOrderS = {0};

	initEditGiacenza(conn, &editGiacenzaS);
	initNewBuyOrder(conn, &newBuyOrderS);




	printf("Si\n");
	newBuyOrderS.idFornitore = 1;

	//execGenericStmt((void *)&newBuyOrder, &newBuyOrder.outParams[0]);
	EXEC_STMT(&newBuyOrderS);

	mysql_stmt_fetch(newBuyOrderS.stmt);
	printf("%lu\n", newBuyOrderS.outBuyOrderId);

}
