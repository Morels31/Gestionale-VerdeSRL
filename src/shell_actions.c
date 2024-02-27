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
	
	while(-1 == (st->esotica = choice("Is it esotic? [y/n]: ", "y", "n"))) printf("There is no default option, retry..\n"); 
	while(-1 == (st->giardino = choice("Select beetween the two [giardino/appartamento]: ", "giardino", "appartamento"))) printf("There is no default option, retry..\n"); 

	while(-1 == (st->fiorita = choice("Select beetween the two [fiorita/verde]: ", "fiorita", "verde"))) printf("There is no default option, retry..\n"); 


	if(!EXEC_STMT(st)) printf("\nNew Specie added correctly\n");
}



void setPrezzo(setPrezzoStruct *st){
	readPKSpecie(st->nomeLatino, st->colore, \
		&st->inParams[0].buffer_length, &st->inParams[1].buffer_length);

	st->newPrezzo = readInt("Insert new price: ", 0, MAX_MYSQL_UINT);

	if(!EXEC_STMT(st)) printf("\nNew Price set correctly\n");
}



void changePassword(changePasswordStruct *st){
	readPassword("Insert new password: ", st->newPsw);
	st->inParams[0].buffer_length = strlen(st->newPsw);

	if(!EXEC_STMT(st)) printf("\nPassword changed successfully\n");
}


