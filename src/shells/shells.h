#pragma once

#include "../common.h"
#include "../statements.h"
#include "../db_utility.h"
#include "../utility.h"
#include "../stmt_utility.h"
#include "../shell_actions.h"



void shellManager(MYSQL *conn);
void shellMagazzino(MYSQL *conn);
