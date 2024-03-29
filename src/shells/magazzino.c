#include "shells.h"



void shellMagazzino(MYSQL *conn){
	getSpecieStruct getSpecieS = {0};
	editGiacenzaStruct editGiacenzaS = {0};
	getFornitoriForSpecieStruct getFornitoriForSpecieS = {0};
	newBuyOrderStruct newBuyOrderS = {0};
	addSpecieToBuyOrderStruct addSpecieToBuyOrderS = {0};
	changePasswordStruct changePasswordS = {0};

	initGetSpecie(conn, &getSpecieS);
	initEditGiacenza(conn, &editGiacenzaS);
	initGetFornitoriForSpecie(conn, &getFornitoriForSpecieS);
	initNewBuyOrder(conn, &newBuyOrderS);
	initAddSpecieToBuyOrder(conn, &addSpecieToBuyOrderS);
	initChangePassword(conn, &changePasswordS);

	printf("\n\n%sSuccessfully logged in with role 'magazzino'.%s\n", GREEN_COLOR, RESET_COLOR);


	unsigned loop = 1;
	unsigned selection;
	while(loop){
		selection = multipleChoice("\nSelect beetween those actions: ", "Get specie", "Edit stock quantity", "Search which fornitori have a specific specie", "New buy order", "Add specie to buy order", "Clear screen", "Change password", "Quit", NULL);

		switch(selection) {
			case 1:
				EXEC_STMT(&getSpecieS);
				break;
			case 2:
				editGiacenza(&editGiacenzaS);
				break;
			case 3:
				getFornitoriForSpecie(&getFornitoriForSpecieS);
				break;
			case 4:
				newBuyOrder(&newBuyOrderS);
				break;
			case 5:
				addSpecieToBuyOrder(&addSpecieToBuyOrderS, newBuyOrderS.outBuyOrderId);
				break;
			case 6:
				clearScreen();
				break;
			case 7:
				changePassword(&changePasswordS);
				break;
			case 8:
				loop = 0;
				break;
			default:
				err("should never happen");
				break;
			}
	}

	mysql_stmt_close(getSpecieS.stmt);
	mysql_stmt_close(editGiacenzaS.stmt);
	mysql_stmt_close(getFornitoriForSpecieS.stmt);
	mysql_stmt_close(newBuyOrderS.stmt);
	mysql_stmt_close(addSpecieToBuyOrderS.stmt);
	mysql_stmt_close(changePasswordS.stmt);
}
