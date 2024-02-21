#pragma once

#include <stdio.h>
#include <stdlib.h>

void dumpResult(MYSQL *conn);
void getCurrentRole(MYSQL *conn, char *dest);
MYSQL *dbLogin(dbI *dbInfo);
void adminShell(MYSQL *conn);
