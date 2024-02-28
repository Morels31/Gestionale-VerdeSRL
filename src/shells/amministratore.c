#include "shells.h"



void shellAmministratore(MYSQL *conn){
	
	addNewUserStruct addNewUserS = {0};
	dropUserStruct dropUserS = {0};
	changePasswordStruct changePasswordS = {0};

	initAddNewUser(conn, &addNewUserS);
	initDropUser(conn, &dropUserS);
	initChangePassword(conn, &changePasswordS);

	printf("\n\n%sSuccessfully logged in with role 'amministratore'.%s\n", GREEN_COLOR, RESET_COLOR);


	unsigned loop = 1;
	unsigned selection;
	while(loop){
		selection = multipleChoice("\nSelect beetween those actions: ", "Add/Overwrite user", "Delete user", "Clear screen", "Change password", "Quit", NULL);

		switch(selection) {
			case 1:
				addNewUser(&addNewUserS);
				break;
			case 2:
				dropUser(&dropUserS);
				break;
			case 3:
				clearScreen();
				break;
			case 4:
				changePassword(&changePasswordS);
				break;
			case 5:
				loop = 0;
				break;
			default:
				err("should never happen");
				break;
			}
	}

	mysql_stmt_close(addNewUserS.stmt);
	mysql_stmt_close(dropUserS.stmt);
	mysql_stmt_close(changePasswordS.stmt);
}
