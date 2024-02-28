#include "common.h"
#include "stmt_utility.h"


int execStmt(MYSQL_STMT *stmt, MYSQL_BIND *inParams, MYSQL_BIND *outParams, int (*postExecFunc)(MYSQL_STMT *, MYSQL_BIND *)){

	if(inParams){
		if(mysql_stmt_bind_param(stmt, inParams)) stmtError(stmt, "mysql_stmt_bind_param() failed");
	}

	if(mysql_stmt_execute(stmt)) stmtErr(stmt, "mysql_stmt_execute() failed");

	if(outParams){
		if(mysql_stmt_bind_result(stmt, outParams)) stmtError(stmt, "mysql_stmt_bind_result() failed");
	}

	if(postExecFunc){
		return (*postExecFunc)(stmt, outParams);
	}

	return 0;
}


int saveOutput(MYSQL_STMT *stmt, MYSQL_BIND *ignored){
	if(mysql_stmt_store_result(stmt)) stmtError(stmt, "mysql_stmt_store_result() failed");

	//potrebbero non esserci risultati
	int tmp = mysql_stmt_fetch(stmt);
	if(tmp){
		if(tmp==MYSQL_NO_DATA){
			printf("Received empty result\n\n");
			if(mysql_stmt_reset(stmt)) stmtError(stmt, "mysql_stmt_reset() failed");
			return 1;
		}
		stmtError(stmt, "mysql_stmt_fetch() failed");
	}
	if(mysql_stmt_reset(stmt)) stmtError(stmt, "mysql_stmt_reset() failed");
	return 0;
}

void printBindedVar(MYSQL_BIND *bind, int padding){
	if(bind->buffer_type == MYSQL_TYPE_STRING || bind->buffer_type == MYSQL_TYPE_VAR_STRING){
		printf(" %-*s |", padding, bind->buffer);
	}
	else if(bind->buffer_type == MYSQL_TYPE_TINY){
		if(bind->is_unsigned) printf(" %-*u |", padding, *(unsigned char *) bind->buffer);
		else printf(" %-*d |", padding, *(char *)bind->buffer);
	}
	else if(bind->buffer_type == MYSQL_TYPE_LONG){
		if(bind->is_unsigned) printf(" %-*lu |", padding, *(unsigned long *)bind->buffer);
		else printf(" %-*ld |", padding, *(long *)bind->buffer);
	}
	else{
		printf("ERROR: UNSUPPORTED VAR TYPE");
	}
}



void printDashes(MYSQL_RES *meta){
	MYSQL_FIELD *field;

	putchar('+');
	for(unsigned i=0; i<mysql_num_fields(meta); i++){

		field = mysql_fetch_field_direct(meta, i);
		for(unsigned j=0; j<field->max_length+2; j++) putchar('-');

		putchar('+');
	}
	putchar('\n');
}


/*
 *  Prints the header of the result set
 *  (the name of the columns)
 */

void printResultSetHeader(MYSQL_RES *meta){
	MYSQL_FIELD *field;
	unsigned colLen, i;

	unsigned nFields = mysql_num_fields(meta);
	for(i=0; i<nFields; i++){

		field = mysql_fetch_field_direct(meta, i);
		colLen = strlen(field->name);

		if(colLen < field->max_length) colLen = field->max_length;
		if(colLen < 4 && !IS_NOT_NULL(field->flags)) colLen = 4;

		field->max_length = colLen;
	}

	printDashes(meta);
	putchar('|');

	for(i=0; i<nFields; i++){

		field = mysql_fetch_field_direct(meta, i);
		printf(" %-*s |", (int)field->max_length, field->name);

	}

	putchar('\n');
	printDashes(meta);
}



int printResultSet(MYSQL_STMT *stmt, MYSQL_BIND *outParams){
	if(mysql_stmt_store_result(stmt)) stmtError(stmt, "mysql_stmt_store_result() failed");

	MYSQL_RES *meta = mysql_stmt_result_metadata(stmt);
	if(!meta){
		printf("ERROR: no metadata exists for this prepared statement");
		return 1;
	}

	unsigned nFields = mysql_stmt_field_count(stmt);
	if(!nFields) err("this statement does not produce a result set!");

	printf("\n\n");
	printResultSetHeader(meta);

	MYSQL_FIELD *field;
	while(!mysql_stmt_fetch(stmt)){

		putchar('|');
		for(unsigned i=0; i<nFields; i++){

			field = mysql_fetch_field_direct(meta, i);

			if(outParams[i].is_null && *outParams[i].is_null)
				printf(" %-*s |", (int)field->max_length, "NULL");
			else
				printBindedVar(&outParams[i], (int)field->max_length);
		}
		putchar('\n');
	}

	printDashes(meta);
	mysql_free_result(meta);
	return 0;
}






