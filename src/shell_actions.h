#pragma once



void addSpecie(addSpecieStruct *st);
void setPrezzo(setPrezzoStruct *st);
void editGiacenza(editGiacenzaStruct *st);
void getFornitoriForSpecie(getFornitoriForSpecieStruct *st);
void newBuyOrder(newBuyOrderStruct *st);
void addSpecieToBuyOrder(addSpecieToBuyOrderStruct *st, unsigned long lastBuyOrderId);
void newSellOrder(newSellOrderStruct *st);
void addSpecieToSellOrder(addSpecieToSellOrderStruct *st, unsigned long lastSellOrderId);
void getCostoOrdine(getCostoOrdineStruct *st, unsigned long lastSellOrderId);
void confirmPayment(confirmPaymentStruct *st, unsigned long lastSellOrderId);
void changePassword(changePasswordStruct *st);
void addNewUser(addNewUserStruct *st);
void dropUser(dropUserStruct *st);
void clearScreen();
