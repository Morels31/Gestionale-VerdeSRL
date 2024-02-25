#pragma once

#include "../common.h"
#include "../statements.h"
#include "../db_utility.h"
#include "../utility.h"



void askAddSpecie(void);
void askSetPrezzo(void);
void askChangePassword(void);
void shellManager(MYSQL *conn);
void shellMagazzino(MYSQL *conn);
