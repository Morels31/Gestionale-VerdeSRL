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

	readInt("Insert how much to modify the stock (with minus or plus signs): ", &editGiacenza.diffGiacenza);

	execGenericStmt((void *)&editGiacenza, NULL);



	printf("Si\n");
	newBuyOrder.idFornitore = 1;

	execGenericStmt((void *)&newBuyOrder, &newBuyOrder.outParams[0]);

	mysql_stmt_fetch(newBuyOrder.stmt);
	printf("%lu\n", newBuyOrder.outBuyOrderId);

}
