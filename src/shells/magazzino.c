#include "../common.h"
#include "magazzino.h"
#include "../statements.h"
#include "../utility.h"

editGiacenzaStruct editGiacenza = {0};


void shellMagazzino(MYSQL *conn){
	initEditGiacenza(conn, &editGiacenza);
	
	readPKSpecie(editGiacenza.nomeLatino, editGiacenza.colore, \
		&editGiacenza.param[0].buffer_length, &editGiacenza.param[1].buffer_length);

	readInt("Insert how much to modify the stock (with minus or plus signs): ", &editGiacenza.diffGiacenza);

	execGenericStmt((void *)&editGiacenza);
}
