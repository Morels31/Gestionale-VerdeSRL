#pragma once

#include <mysql.h>



typedef struct genericStatementStruct{
		MYSQL_STMT *stmt;
		MYSQL_BIND inParams[];
} genericStmtStruct;



typedef struct addSpecieStatementStruct {
        MYSQL_STMT *stmt;
        MYSQL_BIND inParams[6];
        char nomeLatino[MAX_NOME_LATINO_LEN+1];
        char colore[MAX_COLORE_LEN+1];
        char nomeComune[MAX_NOME_COMUNE_LEN+1];
        signed char esotica;
        signed char giardino;
        signed char fiorita;
} addSpecieStruct;



typedef struct setPrezzoStatementStruct {
        MYSQL_STMT *stmt;
        MYSQL_BIND inParams[3];
        char nomeLatino[MAX_NOME_LATINO_LEN+1];
        char colore[MAX_COLORE_LEN+1];
        unsigned long newPrezzo;
} setPrezzoStruct;



typedef struct editGiacenzaStatementStruct {
        MYSQL_STMT *stmt;
        MYSQL_BIND inParams[3];
        char nomeLatino[MAX_NOME_LATINO_LEN+1];
        char colore[MAX_COLORE_LEN+1];
        long diffGiacenza;
} editGiacenzaStruct;



//ha un result set
typedef struct getFornitoriForSpecieStatementStruct {
        MYSQL_STMT *stmt;
        MYSQL_BIND inParams[2];
        MYSQL_BIND outParams[2];
        char nomeLatino[MAX_NOME_LATINO_LEN+1];
        char colore[MAX_COLORE_LEN+1];
        unsigned long outIdFornitore;
        char outNomeFornitore[MAX_NOME_FORNITORE_LEN+1];
} getFornitoriForSpecieStruct;



//il secondo attributo è in OUT
typedef struct newBuyOrderStatementStruct {
        MYSQL_STMT *stmt;
        MYSQL_BIND inParams[1];
        MYSQL_BIND outParams[1];
        unsigned long idFornitore;
        unsigned long outBuyOrderId;
} newBuyOrderStruct;



typedef struct addSpecieToBuyOrderStatementStruct {
        MYSQL_STMT *stmt;
        MYSQL_BIND inParams[4];
        char nomeLatino[MAX_NOME_LATINO_LEN+1];
        char colore[MAX_COLORE_LEN+1];
        unsigned long quantita;
        unsigned long buyOrderId;
} addSpecieToBuyOrderStruct;



//il terzo attributo è in OUT
typedef struct newSellOrderStatementStruct {
        MYSQL_STMT *stmt;
        MYSQL_BIND inParams[2];
        MYSQL_BIND outParams[1];
        char pIVA[MAX_IVA_LEN+1];
        char contatto[MAX_CONTATTO_LEN+1];
        unsigned long outSellOrderId;
} newSellOrderStruct;



typedef struct addSpecieToSellOrderStatementStruct {
        MYSQL_STMT *stmt;
        MYSQL_BIND inParams[4];
        char nomeLatino[MAX_NOME_LATINO_LEN+1];
        char colore[MAX_COLORE_LEN+1];
        unsigned long quantita;
        unsigned long sellOrderId;
} addSpecieToSellOrderStruct;



//il secondo attributo è in OUT
typedef struct getCostoOrdineStatementStruct {
        MYSQL_STMT *stmt;
        MYSQL_BIND inParams[1];
        MYSQL_BIND outParams[1];
        unsigned long sellOrderId;
        unsigned long outCost;
} getCostoOrdineStruct;



typedef struct confirmPaymentStatementStruct {
        MYSQL_STMT *stmt;
        MYSQL_BIND inParams[1];
        unsigned long sellOrderId;
} confirmPaymentStruct;



typedef struct changePasswordStatementStruct {
        MYSQL_STMT *stmt;
        MYSQL_BIND inParams[1];
        char newPsw[MAX_PASSWORD_LEN+1];
} changePasswordStruct;



typedef struct addNewUserStatementStruct {
        MYSQL_STMT *stmt;
        MYSQL_BIND inParams[6];
        char username[MAX_USERNAME_LEN+1];
        char password[MAX_PASSWORD_LEN+1];
        char role[MAX_ROLE_LEN+1];
} addNewUserStruct;



typedef struct dropUserStatementStruct {
        MYSQL_STMT *stmt;
        MYSQL_BIND inParams[1];
        char username[MAX_USERNAME_LEN+1];
} dropUserStruct;



typedef struct getSpecieStatementStruct {
        MYSQL_STMT *stmt;
        MYSQL_BIND outParams[8];
        char nomeLatino[MAX_NOME_LATINO_LEN+1];
        char colore[MAX_COLORE_LEN+1];
        char nomeComune[MAX_NOME_COMUNE_LEN+1];
        long giacenza;
        unsigned long prezzo;
        signed char esotica;
        char giardAppart[30];
        char verdeFiorita[15];
} getSpecieStruct;







int execGenericStmt(genericStmtStruct *st, MYSQL_BIND *optionalOutParams);
void initAddSpecie(MYSQL *conn, addSpecieStruct *st);
void initSetPrezzo(MYSQL *conn, setPrezzoStruct *st);
void initEditGiacenza(MYSQL *conn, editGiacenzaStruct *st);
void initGetFornitoriForSpecie(MYSQL *conn, getFornitoriForSpecieStruct *st);
void initNewBuyOrder(MYSQL *conn, newBuyOrderStruct *st);
void initAddSpecieToBuyOrder(MYSQL *conn, addSpecieToBuyOrderStruct *st);
void initNewSellOrder(MYSQL *conn, newSellOrderStruct *st);
void initAddSpecieToSellOrder(MYSQL *conn, addSpecieToSellOrderStruct *st);
void initGetCostoOrdine(MYSQL *conn, getCostoOrdineStruct *st);
void initConfirmPayment(MYSQL *conn, confirmPaymentStruct *st);
void initChangePassword(MYSQL *conn, changePasswordStruct *st);
void initAddNewUser(MYSQL *conn, addNewUserStruct *st);
void initDropUser(MYSQL *conn, dropUserStruct *st);
void initGetSpecie(MYSQL *conn, getSpecieStruct *st);


