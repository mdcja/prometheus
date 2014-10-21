/* Author: Julian Martinez del Campo
 *
 * This file provides a hash table abstract data type. The goal of this file is
 * to provide an easy to use and flexible hash table.
 *
 * if _EMBEDDED macro is defined then any functions depending on stdio.h are
 * disabled. This may be useful when programming on embedded devices.
 */
#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

#ifndef _EMBEDDED
#include <stdio.h>
#endif /* _EMBEDDED */

#include "attributes.h"

typedef struct _hash_table_type hash_table_t;

/* Create a hash table. It takes in key attributes (for handling different key
 * types) and data attributes (for handling different data types) and finally a
 * capacity. If any of the attributes are set to NULL then it uses the default
 * attributes, which handle pointers. It returns a new hash table on success,
 * else returns NULL.
 */
hash_table_t * hash_table_create( const attr_t * kattr, const attr_t * dattr, int capacity );

/* Destroy a hash table. It deallocates data in the hash table according to the
 * attributes, and destroys the hash table. It returns 0 on success, else
 * returns -1 on error.
 */
int hash_table_destroy( hash_table_t ** ht );

/* Clear all data in the hash table. It deallocates all data in the hash table,
 * clearing the hash table. It returns 0 on success, else returns -1 on error.
 */
int hash_table_clear( hash_table_t ** ht );

/* Resize a hash table. This function allows you to resize the hash table to a
 * different capacity. If there are more items in the hash table than the new
 * capacity, the resize will fail. It returns 0 on success, else returns -1 on
 * error.
 */
int hash_table_reserve( hash_table_t ** ht, int new_capacity );

/* Insert an item into the hash table. Returns 0 on success, else returns -1 on
 * error.
 */
int hash_table_insert( hash_table_t ** ht, void * key, void * data );

/* Look up an item in the hash table. Returns the item if it is in the table,
 * else returns NULL.
 */
void * hash_table_lookup( const hash_table_t * ht, void * key );

/* Remove an item from the hash table. Returns 0 on success, else returns -1 on
 * error.
 */
int hash_table_remove( hash_table_t ** ht, void * key );

#ifndef _EMBEDDED
/* Print information about the hash table as well as all the items and
 * associated keys in the table. Returns the number of characters printed, else
 * returns -1 on error.
 */
int hash_table_print( const hash_table_t * ht, FILE * fp );
#endif /* _EMBEDDED */

/* Get the load on the hash table. It is recommended that the load does not
 * exceed 0.80 or 80% in order to maintain performance. Returns a range between
 * 0 and 1.
 */
double hash_table_load( const hash_table_t * ht );

/* Get the capacity of the hash table. Returns the capacity, else if an error
 * occurs then return -1.
 */
int hash_table_capacity( const hash_table_t * ht );

/* Get the number of items in the hash table. Returns the number of items in
 * the table, else returns -1 on error.
 */
int hash_table_count( const hash_table_t * ht );

/* Get the memory usage of the hash table. It returns the amount of memory used
 * by the hash table (in bytes), else returns -1 on error.
 */
int hash_table_memory( const hash_table_t * ht );

#endif /* _HASH_TABLE_H_ */
