#include "shells.h"



static getSpecieStruct getSpecie = {0};
static addSpecieStruct addSpecie = {0};
static setPrezzoStruct setPrezzo = {0};
static changePasswordStruct changePassword = {0};



void askAddSpecie(void){
	readPKSpecie(addSpecie.nomeLatino, addSpecie.colore, \
		&addSpecie.inParams[0].buffer_length, &addSpecie.inParams[1].buffer_length);

	while(!readLine("Enter nome comune: ", "String too long\n", MAX_NOME_COMUNE_LEN, addSpecie.nomeComune, &addSpecie.inParams[2].buffer_length)) printf("Can't be empty!\n");
	
	while(-1 == (addSpecie.esotica = choice("Is it esotic? [y/n]: ", "y", "n"))) printf("There is no default option, retry..\n"); 
	while(-1 == (addSpecie.giardino = choice("Select beetween the two [giardino/appartamento]: ", "giardino", "appartamento"))) printf("There is no default option, retry..\n"); 

	while(-1 == (addSpecie.fiorita = choice("Select beetween the two [fiorita/verde]: ", "fiorita", "verde"))) printf("There is no default option, retry..\n"); 


	//if(!execGenericStmt((void *) &addSpecie, NULL)) printf("\nNew Specie added correctly\n\n");
	if(!EXEC_STMT(addSpecie)) printf("\nNew Specie added correctly\n");
}


void askSetPrezzo(void){
	readPKSpecie(setPrezzo.nomeLatino, setPrezzo.colore, \
		&setPrezzo.inParams[0].buffer_length, &setPrezzo.inParams[1].buffer_length);

	setPrezzo.newPrezzo = readInt("Insert new price: ", 0, MAX_MYSQL_UINT);

	if(!EXEC_STMT(setPrezzo)) printf("\nNew Price set correctly\n");
}




void askChangePassword(void){
	readPassword("Insert new password: ", changePassword.newPsw);
	changePassword.inParams[0].buffer_length = strlen(changePassword.newPsw);

	if(!EXEC_STMT(changePassword)) printf("\nPassword changed successfully\n");
}




void shellManager(MYSQL *conn){
	initGetSpecie(conn, &getSpecie);
	initAddSpecie(conn, &addSpecie);
	initSetPrezzo(conn, &setPrezzo);
	initChangePassword(conn, &changePassword);

	printf("\nSuccessfully logged in as a manager.\n");


	unsigned loop = 1;
	unsigned selection;
	while(loop){
		selection = multipleChoice("\nSelect beetween those actions: ", "Get Specie", "Add Specie", "Set Prezzo Specie", "Change Password", "Quit", NULL);

		switch(selection) {
			case 1:
				EXEC_STMT(getSpecie);
				break;
			case 2:
				askAddSpecie();
				break;
			case 3:
				askSetPrezzo();
				break;
			case 4:
				askChangePassword();
				memset(changePassword.newPsw, 0, MAX_PASSWORD_LEN);
				break;
			case 5:
				loop = 0;
				break;
			default:
				err("should never happen");
				break;
			}
	}

	mysql_stmt_close(getSpecie.stmt);
	mysql_stmt_close(addSpecie.stmt);
	mysql_stmt_close(setPrezzo.stmt);
	mysql_stmt_close(changePassword.stmt);
}
