/**
 * This file provides data attributes.
 *
 * @brief Provides data attributes.
 * @author Julian Martinez del Campo
 * @license GNU Lesser General Public License (LGPL) https://www.gnu.org/copyleft/lesser.html
 *
 * The data attributes are used to manipulate the data stored in the abstract
 * data types. By changing the attributes, data can be dealt with in a more
 * controlled way; i.e. printing or copying a string.
 */
#ifndef _ATTRIBUTES_H_
#define _ATTRIBUTES_H_

#include <stdio.h>

/**
 * Data attributes.
 *
 * This structure holds the attributes that are commonly used when dealing with
 * data.
 */
struct _data_attributes_type {
    int (*compare)( const void *, const void * );
    int (*print)( const void *, FILE * );
    void * (*copy)( const void * );
    void (*free)( void * );
    unsigned long (*hash)( const void * );
};

typedef struct _data_attributes_type attr_t;

/**
 * Initialize attributes.
 *
 * This function initializes a data attributes structure. By default the
 * attributes manipulate pointer values.
 * 
 * @post If an error occurs then error_code is set (see error.h).
 * @post The all data attributes are initialized to default values, and is safe
 * to use in a data structure.  
 * @param attr the attributes to initialize.
 * @return 0 on success, return -1 on error.
 */
int attr_init( attr_t * attr );

/**
 * Set the compare function.
 *
 * This function sets the data comparison function. The data comparator takes
 * two parameters, data A and data B. It then returns -1,0,1 if A is less than,
 * equal to, or greater than B.
 *
 * @post If an error occurs, then error_code is set (see error.h).
 * @param attr the attributes to set.
 * @param data_compare the comparator function.
 * @return 0 on success, return -1 on error.
 */
int attr_set_compare( attr_t * attr, int (*data_compare)( const void *, const void * ) );

/**
 * Set the print function.
 *
 * This function sets the data print function. The data print function takes
 * two parameters, data, and a file pointer to print to. It then returns the
 * number of characters printed to the stream, or -1 if an error occurred.
 *
 * @param attr the attributes to set.
 * @param data_print the data print function.
 * @return 0 on success, return -1 on error.
 */
int attr_set_print( attr_t * attr, int (*data_print)( const void *, FILE * ) );

/**
 * Set the copy function.
 *
 * This function sets the data copy function. The copy function takes one
 * parameter, data, and it then returns a pointer to a copy of the data.
 *
 * @param attr the attributes to set.
 * @param data_copy the data copy function.
 * @return 0 on success, return -1 on error.
 */
int attr_set_copy( attr_t * attr, void * (*data_copy)( const void * ) );

/**
 * Set the free function.
 *
 * This function sets the data free function. The free function takes one
 * parameter, data, and it then deallocates it from memory.
 *
 * @param attr the attributes to set.
 * @param data_free the data deallocation function.
 * @return 0 on success, return -1 on error.
 */
int attr_set_free( attr_t * attr, void (*data_free)( void * ) );

/**
 * Set the hash function.
 *
 * This function sets the data hash function. The hash function takes one
 * parameter, data, and it then hashes the data and returns the hashed value as
 * an unsigned long integer.
 *
 * @param attr the attributes to set.
 * @param data_hash the data hash function.
 * @param 0 on success, return -1 on error.
 */
int attr_set_hash( attr_t * attr, unsigned long (*data_hash)( const void * ) );

#endif /* _ATTRIBUTES_H_ */
