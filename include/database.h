#ifndef DATABASE_H
#define DATABASE_H

/**
 * @struct Record
 * @brief Represents a single key-value record in the database.
 *
 * This structure may be extended to store more complex data.
 */
typedef struct {
	char* key;		// Key for the data (e.g., "username")
	char* value;	// Value associated with the key
} Record;

/**
 * @brief Initializes the database system.
 *
 * This function sets up the internal database (e.g., in-memory hash table, flat
 * file, etc.). Should be called once at server startup.
 */
void InitDatabase();

/**
 * @brief Retrieves the value for a given key.
 *
 * If the key exists in the database, this returns a dynamically allocated
 * string containing the value. The caller must free the returned string.
 *
 * @param key The key to look up.
 * @return A malloc'd copy of the value, or NULL if not found.
 */
char* DatabaseGet(const char* key);

/**
 * @brief Stores or updates a key-value pair in database.
 *
 * @param key The key to set.
 * @param value The value to associate with the key.
 * @return 1 if successful, 0 otherwise.
 */
int DatabasePost(const char* key, const char* value);

/**
 * @brief Deletes a key-value pair from database.
 *
 * If the key does not exist, this operation is ignored.
 *
 * @param key The key to delete.
 * @return 1 if successful, 0 otherwise.
 */
int DatabaseDelete(const char* key);

/**
 * @brief Cleans up the database system.
 *
 * Frees any dynamically allocated memory and resets internal structures.
 */
void CleanupDatabase();

#endif	// DATABASE_H

// include/database.h