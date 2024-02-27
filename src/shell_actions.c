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
		printf("\nNew Specie added correctly\n");
}



void setPrezzo(setPrezzoStruct *st){
	readPKSpecie(st->nomeLatino, st->colore, \
		&st->inParams[0].buffer_length, &st->inParams[1].buffer_length);

	st->newPrezzo = readInt("Enter new price: ", 0, MAX_MYSQL_UINT);

	if(!EXEC_STMT(st))
		printf("\nNew Price set correctly\n");
}



void editGiacenza(editGiacenzaStruct *st){
	readPKSpecie(st->nomeLatino, st->colore, \
		&st->inParams[0].buffer_length, &st->inParams[1].buffer_length);

	st->diffGiacenza = readInt("Enter how much to modify the stock (with minus or plus signs): ", MIN_MYSQL_INT, MAX_MYSQL_INT);

	if(!EXEC_STMT(st))
		printf("\nGiacenza edited correctly\n");
}



void getFornitoriForSpecie(getFornitoriForSpecieStruct *st){
	readPKSpecie(st->nomeLatino, st->colore, \
		&st->inParams[0].buffer_length, &st->inParams[1].buffer_length);

	EXEC_STMT(st);
}



void newBuyOrder(newBuyOrderStruct *st){

	st->idFornitore = readInt("Enter an idFornitore: ", 1, MAX_MYSQL_UINT);

	if(!EXEC_STMT(st))
		printf("\nNew buy order created successfully.\nWith ID: %lu\n", st->outBuyOrderId);
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
		printf("\nSuccessfully added specie to buy order.\n");
}



void newSellOrder(newSellOrderStruct *st){

	readLine("Enter a P.IVA: ", "String too long\n", MAX_IVA_LEN, st->pIVA, &st->inParams[0].buffer_length);

	readLine("Enter a shipping contact: ", "String too long\n", MAX_CONTATTO_LEN, st->contatto, &st->inParams[1].buffer_length);

	if(!EXEC_STMT(st))
		printf("\nNew sell order created successfully.\nWith ID: %lu\n", st->outSellOrderId);
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
		printf("\nSuccessfully added specie to sell order.\n");
}



void getCostoOrdine(getCostoOrdineStruct *st, unsigned long lastSellOrderId){

	if(lastSellOrderId \
		&& choice("Get price of last created order? [Y/n]: ", 1, "y", "n") == 1)
			st->sellOrderId = lastSellOrderId;
	else
		st->sellOrderId = readInt("Enter a valid sell order ID: ", 1, MAX_MYSQL_UINT);

	if(!EXEC_STMT(st))
		printf("\nPrice of the sell order: %lu\n", st->outCost);
}



void confirmPayment(confirmPaymentStruct *st, unsigned long lastSellOrderId){

	if(lastSellOrderId \
		&& choice("Confirm payment of last created order? [Y/n]: ", 1, "y", "n") == 1)
			st->sellOrderId = lastSellOrderId;
	else
		st->sellOrderId = readInt("Enter a valid sell order ID: ", 1, MAX_MYSQL_UINT);

	if(!EXEC_STMT(st))
		printf("\nSuccessfully confirmed payment of sell order.\n");
}



void changePassword(changePasswordStruct *st){
	readPassword("Enter new password: ", st->newPsw);
	st->inParams[0].buffer_length = strlen(st->newPsw);

	if(!EXEC_STMT(st)) printf("\nPassword changed successfully\n");
}



void addNewUser(addNewUserStruct *st){
	printf("Not yet implemented\n");
}



void dropUser(dropUserStruct *st){
	printf("Not yet implemented\n");
}
