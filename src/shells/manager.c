#include "shells.h"



void shellManager(MYSQL *conn){
	
	getSpecieStruct getSpecieS = {0};
	addSpecieStruct addSpecieS = {0};
	setPrezzoStruct setPrezzoS = {0};
	changePasswordStruct changePasswordS = {0};

	initGetSpecie(conn, &getSpecieS);
	initAddSpecie(conn, &addSpecieS);
	initSetPrezzo(conn, &setPrezzoS);
	initChangePassword(conn, &changePasswordS);

	printf("\n\n%sSuccessfully logged in with role 'manager'.%s\n", GREEN_COLOR, RESET_COLOR);


	unsigned loop = 1;
	unsigned selection;
	while(loop){
		selection = multipleChoice("\nSelect beetween those actions: ", "Get specie", "Add specie", "Set prezzo specie", "Clear screen", "Change password", "Quit", NULL);

		switch(selection) {
			case 1:
				EXEC_STMT(&getSpecieS);
				break;
			case 2:
				addSpecie(&addSpecieS);
				break;
			case 3:
				setPrezzo(&setPrezzoS);
				break;
			case 4:
				clearScreen();
				break;
			case 5:
				changePassword(&changePasswordS);
				break;
			case 6:
				loop = 0;
				break;
			default:
				err("should never happen");
				break;
			}
	}

	mysql_stmt_close(getSpecieS.stmt);
	mysql_stmt_close(addSpecieS.stmt);
	mysql_stmt_close(setPrezzoS.stmt);
	mysql_stmt_close(changePasswordS.stmt);
}
