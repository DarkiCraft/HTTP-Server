#include "database.h"

#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"

const char* database_file = "database.db";

static sqlite3* database = NULL;

void InitDatabase() {
	if (sqlite3_open(database_file, &database) != SQLITE_OK) {
		(void)fprintf(stderr,
									"Error: In InitDatabase(): sqlite3_open() failed: %s\n",
									sqlite3_errmsg(database));
		exit(EXIT_FAILURE);
	}

	const char* sql =
			"CREATE TABLE IF NOT EXISTS database (roll_num TEXT PRIMARY KEY, name "
			"TEXT);";

	char* error = NULL;
	if (sqlite3_exec(database, sql, 0, 0, &error) != SQLITE_OK) {
		(void)fprintf(
				stderr, "Error: In InitDatabase(): sqlite3_exec() failed: %s\n", error);
		sqlite3_free(error);
		if (sqlite3_close(database) != SQLITE_OK) {
			(void)fprintf(stderr,
										"Error: In InitDatabase(): sqlite3_close() failed: %s\n",
										sqlite3_errmsg(database));
		}
		exit(EXIT_FAILURE);
	}
}

char* DatabaseGet(const char* key) {
	char* value = NULL;
	const char* sql = "SELECT name FROM database WHERE roll_num = ?;";
	sqlite3_stmt* stmt = NULL;

	if (sqlite3_prepare_v2(database, sql, -1, &stmt, 0) != SQLITE_OK) {
		(void)fprintf(stderr,
									"Error: In DatabaseGet(): sqlite3_prepare_v2() failed: %s\n",
									sqlite3_errmsg(database));
		return NULL;
	}

	if (sqlite3_bind_text(stmt, 1, key, -1, SQLITE_STATIC) != SQLITE_OK) {
		(void)fprintf(stderr,
									"Error: In DatabaseGet(): sqlite3_bind_text() failed: %s\n",
									sqlite3_errmsg(database));

		if (sqlite3_finalize(stmt) != SQLITE_OK) {
			(void)fprintf(stderr,
										"Error: In DatabaseGet(): sqlite3_finalize() failed: %s\n",
										sqlite3_errmsg(database));
		}

		return NULL;
	}

	if (sqlite3_step(stmt) != SQLITE_ROW) {
		(void)fprintf(
				stderr,
				"Error: In DatabaseGet(): sqlite3_step() failed or no result: %s\n",
				sqlite3_errmsg(database));

		if (sqlite3_finalize(stmt) != SQLITE_OK) {
			(void)fprintf(stderr,
										"Error: In DatabaseGet(): sqlite3_finalize() failed: %s\n",
										sqlite3_errmsg(database));
		}

		return NULL;
	}

	const char* name = (const char*)sqlite3_column_text(stmt, 0);
	if (name != NULL) {
		value = my_strdup(name);
	} else {
		(void)fprintf(stderr,
									"Error: In DatabaseGet(): sqlite3_column_text() failed\n");
	}

	if (sqlite3_finalize(stmt) != SQLITE_OK) {
		(void)fprintf(stderr,
									"Error: In DatabaseGet(): sqlite3_finalize() failed: %s\n",
									sqlite3_errmsg(database));
	}

	return value;
}

void DatabasePost(const char* key, const char* value) {
	const char* sql =
			"INSERT OR REPLACE INTO database (roll_num, name) VALUES (?, ?);";
	sqlite3_stmt* stmt = NULL;

	if (sqlite3_prepare_v2(database, sql, -1, &stmt, 0) != SQLITE_OK) {
		(void)fprintf(stderr,
									"Error: In DatabasePost(): sqlite3_prepare_v2() failed: %s\n",
									sqlite3_errmsg(database));
		return;
	}

	if (sqlite3_bind_text(stmt, 1, key, -1, SQLITE_STATIC) != SQLITE_OK ||
			sqlite3_bind_text(stmt, 2, value, -1, SQLITE_STATIC) != SQLITE_OK) {
		(void)fprintf(stderr,
									"Error: In DatabasePost(): sqlite3_bind_text() failed: %s\n",
									sqlite3_errmsg(database));

		if (sqlite3_finalize(stmt) != SQLITE_OK) {
			(void)fprintf(stderr,
										"Error: In DatabasePost(): sqlite3_finalize() failed: %s\n",
										sqlite3_errmsg(database));
		}

		return;
	}

	if (sqlite3_step(stmt) != SQLITE_DONE) {
		(void)fprintf(
				stderr,
				"Error: In DatabasePost(): sqlite3_step() failed or no result: %s\n",
				sqlite3_errmsg(database));
	}

	if (sqlite3_finalize(stmt) != SQLITE_OK) {
		(void)fprintf(stderr,
									"Error: In DatabasePost(): sqlite3_finalize() failed: %s\n",
									sqlite3_errmsg(database));
	}
}

void DatabasePut(const char* key, const char* value) {
	DatabasePost(key, value);
}

void DatabaseDelete(const char* key) {
	const char* sql = "DELETE FROM database WHERE roll_num = ?;";
	sqlite3_stmt* stmt = NULL;

	if (sqlite3_prepare_v2(database, sql, -1, &stmt, 0) != SQLITE_OK) {
		(void)fprintf(
				stderr,
				"Error: In DatabaseDelete(): sqlite3_prepare_v2() failed: %s\n",
				sqlite3_errmsg(database));

		return;
	}

	if (sqlite3_bind_text(stmt, 1, key, -1, SQLITE_STATIC) != SQLITE_OK) {
		(void)fprintf(
				stderr,
				"Error: In DatabaseDelete(): sqlite3_bind_text() failed: %s\n",
				sqlite3_errmsg(database));

		if (sqlite3_finalize(stmt) != SQLITE_OK) {
			(void)fprintf(
					stderr,
					"Error: In DatabaseDelete(): sqlite3_finalize() failed: %s\n",
					sqlite3_errmsg(database));
		}

		return;
	}

	if (sqlite3_step(stmt) != SQLITE_DONE) {
		(void)fprintf(
				stderr,
				"Error: In DatabaseDelete(): sqlite3_step() failed or no result: %s\n",
				sqlite3_errmsg(database));
	}

	if (sqlite3_finalize(stmt) != SQLITE_OK) {
		(void)fprintf(stderr,
									"Error: In DatabaseDelete(): sqlite3_finalize() failed: %s\n",
									sqlite3_errmsg(database));
	}
}

void CleanupDatabase() {
	if (sqlite3_close(database) != SQLITE_OK) {
		(void)fprintf(stderr,
									"Error: In CleanupDatabase(): sqlite3_close() failed: %s\n",
									sqlite3_errmsg(database));
	}

	database = NULL;
}

// src/database.c