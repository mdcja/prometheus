/* author: Julian Martinez del Campo
 *
 * This file implementes attributes.h
 *
 * If _VERBOSE > 0 then display error messages.
 */
#ifndef _EMBEDDED
#include <stdio.h>
#endif /* _EMBEDDED */

#include "attributes.h"
#include "error.h"

/* Compare pointers
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

#ifndef _EMBEDDED
/* Print a pointer
 */
static int default_print( const void * data, FILE * fp )
{
    return fprintf( fp, "%p", data );
}
#endif /* _EMBEDDED */

/* Dont copy anything, return a pointer to the object
 */
static void * default_copy( const void * data )
{
    return (void *)data;
}

/* Dont do anyting
 */
static void default_free( void * data )
{
    return;
}

/* CRC hash variant
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

int attr_init( attr_t * attr )
{
    if( attr == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "attr_init: attr" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    /* set functions */
    attr->compare = default_compare;
    attr->copy = default_copy;
    attr->free = default_free;
    attr->hash = default_hash;
#ifndef _EMBEDDED
    attr->print = default_print;
#endif /* _EMBEDDED */
            
    return 0;
}

int attr_set_compare( attr_t * attr, int (*data_compare)( const void *, const void *) )
{
    if( attr == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "attr_set_compare: attr" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    if( data_compare == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "attr_set_compare: data_compare" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    attr->compare = data_compare;

    return 0;
}

int attr_set_print( attr_t * attr, int (*data_print)( const void *, FILE * ) )
{
    if( attr == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "attr_set_print: attr" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */
        
        return -1;
    }

    if( data_print == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "attr_set_print: data_print" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    attr->print = data_print;

    return 0;
}

int attr_set_copy( attr_t * attr, void * (*data_copy)( const void * ) )
{
    if( attr == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "attr_set_copy: attr" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    if( data_copy == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "attr_set_copy: data_copy" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    attr->copy = data_copy;

    return 0;
}

int attr_set_free( attr_t * attr, void (*data_free)( void * ) )
{
    if( attr == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "attr_set_free: attr" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    if( data_free == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "attr_set_free: data_free" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    attr->free = data_free;

    return 0;
}

int attr_set_hash( attr_t * attr, unsigned long (*data_hash)( const void * ) )
{
    if( attr == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "attr_set_hash: attr" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    if( data_hash == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "attr_set_hash: data_hash" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    attr->hash = data_hash;

    return 0;
}
