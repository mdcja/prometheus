/**
 * This file provides an implementation of attributes.
 *
 * @brief Provides an implementation of data attributes.
 * @author Julian Martinez del Campo
 * @license GNU Lesser General Public License (LGPL) https://www.gnu.org/copyleft/lesser.html
 *
 * This file provides an implementation of the data attributes. This file also
 * provides the default data functions.
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "attributes.h"
#include "error.h"

/**
 * Default data comparison function.
 *
 * This function numerically compares the locations pointed to by data1 and
 * data2.
 *
 * @param data1 an object to compare.
 * @param data2 another object to compare.
 * @return -1, 0, 1 if data1 is less than, equal to, or greater than data2.
 * equal to, or greater than each other.
 */
static int default_compare( const void * data1, const void * data2 )
{
    if( data1 == data2 )
    {
        return 0;
    }
    else if( data1 < data2 )
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

/**
 * Default data printing function.
 *
 * This function prints the memory location pointed to by data.
 *
 * @param fp the file to print to.
 * @param data the data to print.
 * @return the number of characters printed to the file, return -1 on error.
 */
static int default_print( const void * data, FILE * fp )
{
    return fprintf( fp, "%p", data );
}

/**
 * Default data copy function.
 *
 * This function returns the memory location pointed to by data.
 *
 * @post the address of data is returned, not a copy.
 * @param data the data to copy.
 * @return the address of data.
 */
static void * default_copy( const void * data )
{
    return (void *)data;
}

/**
 * Default free data function.
 *
 * This function does nothing.
 *
 * @post the data is still in memory.
 * @param data to free.
 */
static void default_free( void * data )
{
    return;
}

/**
 * Integer hash key
 *
 * CRC variant hash
 *
 * @see http://www.cs.hmc.edu/~geoff/classes/hmc.cs070.200101/homework10/hashfuncs.html
 */
static unsigned long default_hash( const void * key )
{
    unsigned long hash = *(int*)key;
    unsigned long high_order = 0;

    high_order = hash & 0xf8000000;
    hash = hash << 5;
    hash = hash ^ ( high_order >> 27 );
    hash = hash ^ 1; /* n */

    return hash;
}

/**
 * Initialize attributes.
 *
 * @post All functions are initialized to the default values.
 */
int attr_init( attr_t * attr )
{
    if( attr == NULL )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "attr_init" );

        return -1;
    }

    attr->compare = default_compare;
    attr->print = default_print;
    attr->copy = default_copy;
    attr->free = default_free;
    attr->hash = default_hash;

    return 0;
}

/**
 * Set the compare function.
 *
 * @post The data compare function is set.
 */
int attr_set_compare( attr_t * attr, int (*data_compare)( const void *, const void *) )
{
    if( attr == NULL )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "attr_set_compare" );

        return -1;
    }

    if( data_compare == NULL )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "attr_set_compare" );

        return -1;
    }

    attr->compare = data_compare;

    return 0;
}

/**
 * Set the print function.
 *
 * @post The data print function is set.
 */
int attr_set_print( attr_t * attr, int (*data_print)( const void *, FILE * ) )
{
    if( attr == NULL )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "attr_set_print" );
        
        return -1;
    }

    if( data_print == NULL )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "attr_set_print" );

        return -1;
    }

    attr->print = data_print;

    return 0;
}

/**
 * Set the copy function.
 *
 * @post The data copy function is set.
 */
int attr_set_copy( attr_t * attr, void * (*data_copy)( const void * ) )
{
    if( attr == NULL )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "attr_set_copy" );

        return -1;
    }

    if( data_copy == NULL )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "attr_set_copy" );

        return -1;
    }

    attr->copy = data_copy;

    return 0;
}

/**
 * Set the free function.
 *
 * @post The data free function is set.
 */
int attr_set_free( attr_t * attr, void (*data_free)( void * ) )
{
    if( attr == NULL )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "attr_set_free" );

        return -1;
    }

    if( data_free == NULL )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "attr_set_free" );

        return -1;
    }

    attr->free = data_free;

    return 0;
}

/**
 * Set the hash function.
 *
 * @post The data hash function is set.
 */
int attr_set_hash( attr_t * attr, unsigned long (*data_hash)(const void *) )
{
    if( attr == NULL )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "attr_set_hash" );

        return -1;
    }

    if( data_hash == NULL )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "attr_set_hash" );

        return -1;
    }

    attr->hash = data_hash;

    return 0;
}
