#include "common.h"
#include "statements.h"
#include "db_utility.h"
#include "utility.h"
#include "stmt_utility.h"
#include "shell_actions.h"



void addSpecie(addSpecieStruct *st){
	readPKSpecie(st->nomeLatino, st->colore, \
		&st->inParams[0].buffer_length, &st->inParams[1].buffer_length);

	while(!readLine("Enter nome comune: ", "String too long\n", MAX_NOME_COMUNE_LEN, st->nomeComune, &st->inParams[2].buffer_length)) printf("Can't be empty!\n");

	unsigned tmp;
	
	tmp = choice("Is it esotic? [Y/n]: ", 1, "y", "n");
	st->esotica = tmp==2 ? 0 : 1;


	tmp = choice("Select beetween the two [GIARDINO/appartamento]: ", 1, "giardino", "appartamento");
	st->giardino = tmp==2 ? 0 : 1;


	tmp = choice("Select beetween the two [fiorita/verde]: ", 0, "fiorita", "verde");
	st->fiorita = tmp==2 ? 0 : 1;


	if(!EXEC_STMT(st))
		printf("\n\n%sNew Specie added correctly.%s\n", GREEN_COLOR, RESET_COLOR);
}



void setPrezzo(setPrezzoStruct *st){
	readPKSpecie(st->nomeLatino, st->colore, \
		&st->inParams[0].buffer_length, &st->inParams[1].buffer_length);

	st->newPrezzo = readInt("Enter new price: ", 0, MAX_MYSQL_UINT);

	if(!EXEC_STMT(st))
		printf("\n\n%sNew Price set correctly.%s\n", GREEN_COLOR, RESET_COLOR);
}



void editGiacenza(editGiacenzaStruct *st){
	readPKSpecie(st->nomeLatino, st->colore, \
		&st->inParams[0].buffer_length, &st->inParams[1].buffer_length);

	st->diffGiacenza = readInt("Enter how much to modify the stock (with minus or plus signs): ", MIN_MYSQL_INT, MAX_MYSQL_INT);

	if(!EXEC_STMT(st))
		printf("\n\n%sGiacenza edited correctly.%s\n", GREEN_COLOR, RESET_COLOR);
}



void getFornitoriForSpecie(getFornitoriForSpecieStruct *st){
	readPKSpecie(st->nomeLatino, st->colore, \
		&st->inParams[0].buffer_length, &st->inParams[1].buffer_length);

	EXEC_STMT(st);
}



void newBuyOrder(newBuyOrderStruct *st){

	st->idFornitore = readInt("Enter an idFornitore: ", 1, MAX_MYSQL_UINT);

	if(!EXEC_STMT(st))
		printf("\n\n%sNew buy order created successfully, with ID: %lu.%s\n", GREEN_COLOR, st->outBuyOrderId, RESET_COLOR);
}



void addSpecieToBuyOrder(addSpecieToBuyOrderStruct *st, unsigned long lastBuyOrderId){
	readPKSpecie(st->nomeLatino, st->colore, \
		&st->inParams[0].buffer_length, &st->inParams[1].buffer_length);

	st->quantita = readInt("Enter quantity: ", 1, MAX_MYSQL_UINT);

	if(lastBuyOrderId \
		&& choice("Add specie to the last created order? [Y/n]: ", 1, "y", "n") == 1)
			st->buyOrderId = lastBuyOrderId;
	else
		st->buyOrderId = readInt("Enter a valid buy order ID: ", 1, MAX_MYSQL_UINT);

	if(!EXEC_STMT(st))
		printf("\n\n%sSuccessfully added specie to buy order with ID: %lu.%s\n", GREEN_COLOR, st->buyOrderId, RESET_COLOR);
}



void newSellOrder(newSellOrderStruct *st){

	readLine("Enter a P.IVA: ", "String too long\n", MAX_IVA_LEN, st->pIVA, &st->inParams[0].buffer_length);

	readLine("Enter a shipping contact: ", "String too long\n", MAX_CONTATTO_LEN, st->contatto, &st->inParams[1].buffer_length);

	if(!EXEC_STMT(st))
		printf("\n\n%sNew sell order created successfully, with ID: %lu.%s\n", GREEN_COLOR, st->outSellOrderId, RESET_COLOR);
}



void addSpecieToSellOrder(addSpecieToSellOrderStruct *st, unsigned long lastSellOrderId){
	readPKSpecie(st->nomeLatino, st->colore, \
		&st->inParams[0].buffer_length, &st->inParams[1].buffer_length);

	st->quantita = readInt("Enter quantity: ", 1, MAX_MYSQL_UINT);

	if(lastSellOrderId \
		&& choice("Add specie to the last created order? [Y/n]: ", 1, "y", "n") == 1)
			st->sellOrderId = lastSellOrderId;
	else
		st->sellOrderId = readInt("Enter a valid sell order ID: ", 1, MAX_MYSQL_UINT);

	if(!EXEC_STMT(st))
		printf("\n\n%sSuccessfully added specie to sell order with ID: %lu.%s\n", GREEN_COLOR, st->sellOrderId, RESET_COLOR);
}



void getCostoOrdine(getCostoOrdineStruct *st, unsigned long lastSellOrderId){

	if(lastSellOrderId \
		&& choice("Get price of last created order? [Y/n]: ", 1, "y", "n") == 1)
			st->sellOrderId = lastSellOrderId;
	else
		st->sellOrderId = readInt("Enter a valid sell order ID: ", 1, MAX_MYSQL_UINT);

	if(!EXEC_STMT(st))
		printf("\n\n%sPrice of the sell order: %lu cents.%s\n", GREEN_COLOR, st->outCost, RESET_COLOR);
}



void confirmPayment(confirmPaymentStruct *st, unsigned long lastSellOrderId){

	if(lastSellOrderId \
		&& choice("Confirm payment of last created order? [Y/n]: ", 1, "y", "n") == 1)
			st->sellOrderId = lastSellOrderId;
	else
		st->sellOrderId = readInt("Enter a valid sell order ID: ", 1, MAX_MYSQL_UINT);

	if(!EXEC_STMT(st))
		printf("\n\n%sSuccessfully confirmed payment of sell order with id %lu.%s\n", GREEN_COLOR, st->sellOrderId, RESET_COLOR);
}



void changePassword(changePasswordStruct *st){
	readPassword("Enter new password: ", st->newPsw);
	st->inParams[0].buffer_length = strlen(st->newPsw);

	if(!EXEC_STMT(st))
		printf("\n\n%sPassword changed successfully.%s\n", GREEN_COLOR, RESET_COLOR);

	memset(st->newPsw, 0, MAX_PASSWORD_LEN);
}



void addNewUser(addNewUserStruct *st){

	readUsername("Enter username of new user: ", st->username);
	st->inParams[0].buffer_length = strlen(st->username);

	readPassword("Enter initial password: ", st->password);
	st->inParams[1].buffer_length = strlen(st->password);

	unsigned sel = multipleChoice("Select role of the new user: ", "Manager" ,"Magazzino", "Segreteria", NULL);
	switch(sel){
		case 1:
			strcpy(st->role, "manager");
			st->inParams[2].buffer_length = strlen("manager");
			break;
		case 2:
			strcpy(st->role, "magazzino");
			st->inParams[2].buffer_length = strlen("magazzino");
			break;
		case 3:
			strcpy(st->role, "segreteria");
			st->inParams[2].buffer_length = strlen("segreteria");
			break;
		default:
			err("should never happen");
			break;
	}

	if(!EXEC_STMT(st))
		printf("\n\n%sNew user '%s' created successfully%s\n", GREEN_COLOR, st->username, RESET_COLOR);

	memset(st->password, 0, MAX_PASSWORD_LEN);
}



void dropUser(dropUserStruct *st){
	readUsername("Enter username of new user: ", st->username);
	st->inParams[0].buffer_length = strlen(st->username);

	if(!EXEC_STMT(st))
		printf("\n\n%sUser '%s' deleted successfully.%s\n", GREEN_COLOR, st->username, RESET_COLOR);
}
