/* Author: Julian Martinez del Campo
 *
 * This file provides data attributes for abstract data types. The goal of
 * having attributes is to allow data structures to also contain the
 * information needed to manipulate (allocate, free, copy, hash, print) data of
 * any type. The programmer configures the attributes for a data type, e.g. a
 * string, and the data structure is able to manipulate strings (in a basic
 * sense).
 *
 * All of the functions set error_code defined in error.h for more detiled
 * error reporting.
 *
 * If the _EMBEDDED macro is enabled then functions requiring stdio.h are
 * disabled. This may be useful when programming on embedded systems.
 */
#ifndef _ATTRIBUTES_H_
#define _ATTRIBUTES_H_

#ifndef _EMBEDDED
#include <stdio.h>
#endif /* _EMBEDDED */

/* Contains function pointers for all basic data operations
 */
typedef struct _data_attributes_type {
    /* The comparator. This function compares data A with data B, if A is
     * greater than B, A is equals to B, or if A is less than B return 1, 0,
     * and -1, respectively.
     */
    int (*compare)( const void *, const void * );

#ifndef _EMBEDDED
    /* Print data. This function prints the data passed to it, to the
     * associated file pointer. It returns the number of characters printed.
     */
    int (*print)( const void *, FILE * );
#endif /* _EMBEDDED */

    /* Copy data. This function allocates (if necessary) and copies data. It
     * should return a pointer to the new copied data.
     */
    void * (*copy)( const void * );

    /* Free data. This function deallocates (if necessary) data.
     */
    void (*free)( void * );

    /* Hash data. This function hashes the data passed to it.
     */
    unsigned long (*hash)( const void * );
} attr_t;

/* Initialize attributes. It assigns default function pointers to the
 * attributes structure that are safe to use with any data type (it deals with
 * pointer values). It returns 0 on success, and returns -1 on error.
 */
int attr_init( attr_t * attr );

/* Set the comparator. It assigns the comparator to the attributes data
 * structure. It returns 0 on success, and returns -1 on error.
 */
int attr_set_compare( attr_t * attr, int (*data_compare)( const void *, const void * ) );

#ifndef _EMBEDDED
/* Set the print function. It assigns the printing function to the attributes
 * data structure. It returns 0 on success, and returns -1 on error.
 */
int attr_set_print( attr_t * attr, int (*data_print)( const void *, FILE * ) );
#endif /* _EMBEDDED */

/* Set the copy function. It assigns the copy function to the attributes
 * structure. It returns 0 on success, and returns -1 on error.
 */
int attr_set_copy( attr_t * attr, void * (*data_copy)( const void * ) );

/* Set the free function. It assigns the free function to the attributes
 * structure. It returns 0 on success, and returns -1 on error.
 */
int attr_set_free( attr_t * attr, void (*data_free)( void * ) );

/* Set the hash function. It assigns the hashing function to the attributes
 * structure. It returns 0 on success, and returns -1 on error.
 */
int attr_set_hash( attr_t * attr, unsigned long (*data_hash)( const void * ) );

#endif /* _ATTRIBUTES_H_ */
