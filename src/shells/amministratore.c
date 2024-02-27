#include "shells.h"



void shellAmministratore(MYSQL *conn){
	
	addNewUserStruct addNewUserS = {0};
	dropUserStruct dropUserS = {0};
	changePasswordStruct changePasswordS = {0};

	initAddNewUser(conn, &addNewUserS);
	initDropUser(conn, &dropUserS);
	initChangePassword(conn, &changePasswordS);

	printf("\nSuccessfully logged in with role 'amministratore'.\n");


	unsigned loop = 1;
	unsigned selection;
	while(loop){
		selection = multipleChoice("\nSelect beetween those actions: ", "Add/Overwrite user", "Delete user", "Change password", "Quit", NULL);

		switch(selection) {
			case 1:
				addNewUser(&addNewUserS);
				break;
			case 2:
				dropUser(&dropUserS);
				break;
			case 3:
				changePassword(&changePasswordS);
				memset(changePasswordS.newPsw, 0, MAX_PASSWORD_LEN);
				break;
			case 4:
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
