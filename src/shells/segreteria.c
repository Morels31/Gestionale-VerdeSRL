#include "shells.h"



void shellSegreteria(MYSQL *conn){
	
	getSpecieStruct getSpecieS = {0};
	newSellOrderStruct newSellOrderS = {0};
	addSpecieToSellOrderStruct addSpecieToSellOrderS = {0};
	getCostoOrdineStruct getCostoOrdineS = {0};
	confirmPaymentStruct confirmPaymentS = {0};
	changePasswordStruct changePasswordS = {0};

	initGetSpecie(conn, &getSpecieS);
	initNewSellOrder(conn, &newSellOrderS);
	initAddSpecieToSellOrder(conn, &addSpecieToSellOrderS);
	initGetCostoOrdine(conn, &getCostoOrdineS);
	initConfirmPayment(conn, &confirmPaymentS);
	initChangePassword(conn, &changePasswordS);

	printf("\nSuccessfully logged in with role 'segreteria'.\n");


	unsigned loop = 1;
	unsigned selection;
	while(loop){
		selection = multipleChoice("\nSelect beetween those actions: ", "Get specie", "New sell order", "Add specie to sell order", "Get order cost", "Confirm order payment", "Change password", "Quit", NULL);

		switch(selection) {
			case 1:
				EXEC_STMT(&getSpecieS);
				break;
			case 2:
				newSellOrder(&newSellOrderS);
				break;
			case 3:
				addSpecieToSellOrder(&addSpecieToSellOrderS, newSellOrderS.outSellOrderId);
				break;
			case 4:
				getCostoOrdine(&getCostoOrdineS, newSellOrderS.outSellOrderId);
				break;
			case 5:
				confirmPayment(&confirmPaymentS, newSellOrderS.outSellOrderId);
				break;
			case 6:
				changePassword(&changePasswordS);
				memset(changePasswordS.newPsw, 0, MAX_PASSWORD_LEN);
				break;
			case 7:
				loop = 0;
				break;
			default:
				err("should never happen");
				break;
			}
	}

	mysql_stmt_close(getSpecieS.stmt);
	mysql_stmt_close(newSellOrderS.stmt);
	mysql_stmt_close(addSpecieToSellOrderS.stmt);
	mysql_stmt_close(getCostoOrdineS.stmt);
	mysql_stmt_close(confirmPaymentS.stmt);
	mysql_stmt_close(changePasswordS.stmt);
}
