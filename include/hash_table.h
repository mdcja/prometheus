/**
 * This file provides a generic hash table ADT.
 *
 * \brief This file provides a hash table ADT
 * \author Julian Martinez del Campo
 * \license GNU Lesser General Public License (LGPL) https://www.gnu.org/copyleft/lesser.html
 *
 * For maximum performance the table size should always be a prime number. See
 * math.h for a prime number generator.
 *
 * The load on the hash table should be below 80% in order to ensure efficient
 * data retrieval.
 */
#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

#include <stdio.h>

#include "attributes.h"

typedef struct _hash_table_type hash_table_t;

/**
 * Create a new hash table.
 *
 * \post If an error occurs, then error_code is set (see error.h).  
 * \param kattr the hash table key attribute. If the attribute is NULL then a
 * hash table is created with the default settings.
 * \param dattr the hash table data attribute. If the attribute is NULL then a
 * hash table is created with the default settings.
 * \param capacity the initial capacity of the hash table. For performance
 * reasons this should be a prime number.
 * \return a new hash table. If an error occurs then return NULL.
 */
hash_table_t * hash_table_create( const attr_t * kattr, const attr_t * dattr, int capacity );

/**
 * Destroy a hash table.
 *
 * \post If an error occurs, then error_code is set (see error.h).  
 * \post The hash table and all data contained in it is deallocated.
 * \param ht a hash table.
 * \return 0 on success, return -1 on error.
 */
int hash_table_destroy( hash_table_t ** ht );

/**
 * Clear a hash table.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \post All data is deallocated and removed from the hash table.
 * \param ht a hash table.
 * \return 0 on success, else return -1 on error
 */
int hash_table_clear( hash_table_t ** ht );

/**
 * Resize a hash table.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \param ht a hash table.
 * \param new_capacity the new capacity to set the hash table to. If new
 * capacity is less than the number of items in the hash table then keep old
 * size.
 * \return 0 on success, return -1 on error.
 */
int hash_table_reserve( hash_table_t ** ht, int new_capacity );

/**
 * Insert/Update item into a hash table.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \post data is inserted into the table.
 * \post If item exists in hash table, then update the item.
 * \param ht a hash table.
 * \param data the data to insert into the table.
 * \return 0 on success, -1 on error.
 */
int hash_table_insert( hash_table_t ** ht, void * key, void * data );

/**
 * Look up item in the table
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \param ht a hash table.
 * \param key a key to look up.
 * \return the data associated with the key, else return NULL.
 */
void * hash_table_lookup( const hash_table_t * ht, void * key );

/**
 * Remove item from hash table.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \post Both the key and its associated data is deallocated and removed from
 * the hash table.  
 * \param ht a hash table.
 * \param key a key value to remove.
 * \return 0 on success, -1 on error.
 */
int hash_table_remove( hash_table_t ** ht, void * key );

/**
 * Print a hash table
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \param ht a hash table.
 * \param fp a file pointer.
 * \return the number of characters printed on success, -1 on error.
 */
int hash_table_print( const hash_table_t * ht, FILE * fp );

/**
 * Get the hash table's load.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \param ht a hash table.
 * \return the load on the hash table.
 */
double hash_table_load( const hash_table_t * ht );

/**
 * Get the capacity of a hash table.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \param ht a hash table.
 * \return the capacity of the hash table.
 */
int hash_table_capacity( const hash_table_t * ht );

/**
 * Get the number of items in a hash table.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \param ht a hash table.
 * \return the number of items in a hash table. return -1 on error.
 */
int hash_table_count( const hash_table_t * ht );

/**
 * Get the memory usage of the hash table.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \param ht a hash table.
 * \return the number of bytes used up by this data structure, return -1 on error.
 */
int hash_table_memory( const hash_table_t * ht );

#endif /* _HASH_TABLE_H_ */
