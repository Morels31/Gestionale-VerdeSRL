#include "../common.h"
#include "magazzino.h"
#include "../statements.h"
#include "../utility.h"



static editGiacenzaStruct editGiacenza = {0};
static newBuyOrderStruct newBuyOrder = {0};



void shellMagazzino(MYSQL *conn){
	initEditGiacenza(conn, &editGiacenza);
	initNewBuyOrder(conn, &newBuyOrder);


	readPKSpecie(editGiacenza.nomeLatino, editGiacenza.colore, \
		&editGiacenza.inParams[0].buffer_length, &editGiacenza.inParams[1].buffer_length);

	editGiacenza.diffGiacenza = readInt("Insert how much to modify the stock (with minus or plus signs): ", MIN_MYSQL_INT, MAX_MYSQL_INT);

	EXEC_STMT(editGiacenza);



	printf("Si\n");
	newBuyOrder.idFornitore = 1;

	//execGenericStmt((void *)&newBuyOrder, &newBuyOrder.outParams[0]);
	EXEC_STMT(newBuyOrder);

	mysql_stmt_fetch(newBuyOrder.stmt);
	printf("%lu\n", newBuyOrder.outBuyOrderId);

}
