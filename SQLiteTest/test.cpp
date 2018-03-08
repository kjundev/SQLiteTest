#pragma comment(lib,"sqlite3.lib")
#include "sqlite3.h"
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <stdlib.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	for (i = 0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

int main(int argc, char* argv[]) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	const char *sql;

	/* Open database */
	rc = sqlite3_open("test.db", &db);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return(0);
	}
	else {
		fprintf(stderr, "Opened database successfully\n");
	}

	/* Create SQL statement */
	sql = "CREATE TABLE COMPANY("  \
		"ID INT PRIMARY KEY     NOT NULL," \
		"NAME           TEXT    NOT NULL," \
		"AGE            INT     NOT NULL," \
		"ADDRESS        CHAR(50)," \
		"SALARY         REAL );";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

	// PRAGMA
	// synchronous = OFF journal_mode=OFF TEMP_store = memory cash_SIZE + !))) PAGE_SIZE + @)$* COUNT_changes=OFF case_SENSITIVE_LIKE+Ffalse
	sqlite3_exec(db, "synchronous = OFF", NULL, NULL, NULL);
	sqlite3_exec(db, "journal_mode = OFF", NULL, NULL, NULL);
	sqlite3_exec(db, "temp_store = memory", NULL, NULL, NULL);
	sqlite3_exec(db, "cash_size = 1000", NULL, NULL, NULL);
	sqlite3_exec(db, "count_changes = OFF", NULL, NULL, NULL);
	sqlite3_exec(db, "case_sensitive_like = false", NULL, NULL, NULL);

	clock_t t = clock();

	// 'db' is the pointer you got from sqlite3_open*
	sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
	
	for (int i = 0; i < 10; i++)
	{
		char query[700];
		const char *insertsql;

		insertsql = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
			"VALUES (%d, 'Paul', 32, 'California', 20000.00 ); ";

		sprintf_s(query, insertsql, i);

		/* Execute SQL statement */
		rc = sqlite3_exec(db, query, callback, 0, &zErrMsg);
	}
	
	// Any (modifying) SQL commands executed here are not committed until at the you call:
	rc = sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, NULL);

	t = clock() - t;
	 
	printf("\n Time Esc :: %f ", ((float)t) / CLOCKS_PER_SEC);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		fprintf(stdout, "Records created successfully\n");
	}
	sqlite3_close(db);
	return 0;
}
