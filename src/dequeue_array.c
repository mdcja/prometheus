/**
 * This file provides an implementation of a dequeue using an array data
 * structure.
 *
 * @brief Provide a dequeue implemented using an array.
 * @author Julian Martinez del Campo
 * @license GNU Lesser General Public License (LGPL) https://www.gnu.org/copyleft/lesser.html
 *
 * This dequeue has a memory complexity of O(1).
 * 
 * This dequeue has the following run time complexity:
 *
 * Operation            | Complexity
 * ---------------------|-----------
 * create               | O(1)
 * destroy              | O(n)
 * reserve              | O(n)
 * clear                | O(n)
 * peek_front           | O(1)
 * peek_back            | O(1)
 * contains             | O(n)
 * insert(push)_front   | O(n)
 * insert(push)_back    | O(1)
 * remove(pop)_front    | O(n)
 * remove(pop)_back     | O(1)
 * remove_item          | O(n)
 * print                | O(n)
 * memory               | O(1)
 * capacity             | O(1)
 * count                | O(1)
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "dequeue.h"
#include "error.h"
#include "attributes.h"

/**
 * A dequeue with an array data structure
 */
struct _dequeue_type {
    void ** dequeue;
    int capacity;
    int count;
    int memory;
    attr_t attr;
};

/**
 * Create a new dequeue.
 */
dequeue_t * dequeue_create( const attr_t * attr, int capacity )
{
    dequeue_t * new_dequeue = NULL;

    /* check */
    if( capacity < 1 )
    {
        error_code = ERROR_INVALID_CAPACITY;
        error_print( "dequeue_create" );

        return NULL;
    }
    
    /* create */
    new_dequeue = (dequeue_t *)calloc( 1, sizeof( dequeue_t ) );
    assert( new_dequeue );
    if( !new_dequeue )
    {
        error_code = ERROR_MEMORY_ALLOCATION_FAILED;
        error_print( "dequeue_create" );

        return NULL;
    }

    new_dequeue->dequeue = (void **)calloc( capacity, sizeof( void * ) );
    assert( new_dequeue->dequeue );
    if( !new_dequeue->dequeue )
    {
        error_code = ERROR_MEMORY_ALLOCATION_FAILED;
        error_print( "dequeue_create" );

        return NULL;
    }

    /* init */
    new_dequeue->capacity = capacity;
    new_dequeue->count = 0;
    new_dequeue->memory = sizeof( dequeue_t ) + capacity * sizeof( void * );
    if( !attr )
    {
        attr_init( &new_dequeue->attr );
    }
    else
    {
        new_dequeue->attr = *attr;
    }

    assert( new_dequeue->attr.copy );
    assert( new_dequeue->attr.free );
    assert( new_dequeue->attr.compare );
    assert( new_dequeue->attr.print );

    return new_dequeue;
}

/**
 * Destroy a dequeue.
 */
int dequeue_destroy( dequeue_t ** dequeue )
{
    /* check */
    if( !dequeue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_destroy" );

        return -1;
    }

    if( !*dequeue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_destroy" );
        
        return -1;
    }

    /* remove all items */
    if( dequeue_clear( dequeue ) != 0 )
    {
        return -1;
    }

    /* deallocate */
    free( (*dequeue)->dequeue );
    (*dequeue)->dequeue = NULL;

    free( *dequeue );
    *dequeue = NULL;

    return 0;
}

/**
 * Resize the dequeue.
 */
int dequeue_reserve( dequeue_t ** dequeue, int capacity )
{
    void ** old_dequeue = NULL;
    void ** new_dequeue = NULL;
    int i = 0;

    /* check */
    if( !dequeue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_reserve" );

        return -1;
    }

    if( !*dequeue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_reserve" );

        return -1;
    }

    if( capacity <= (*dequeue)->count )
    {
        error_code = ERROR_INVALID_CAPACITY;
        error_print( "dequeue_reserve" );

        return -1;
    }

    /* create */
    new_dequeue = (void **)calloc( capacity, sizeof( void * ) );
    assert( new_dequeue );
    if( !new_dequeue )
    {
        error_code = ERROR_MEMORY_ALLOCATION_FAILED;
        error_print( "dequeue_reserve" );

        return -1;
    }

    /* copy */
    for( i = 0; i < (*dequeue)->count; ++i )
    {
        new_dequeue[ i ] = (*dequeue)->dequeue[ i ];
    }

    /* replace */
    old_dequeue = (*dequeue)->dequeue;
    (*dequeue)->dequeue = new_dequeue;
    (*dequeue)->memory = sizeof( dequeue_t ) + capacity * sizeof( void * );
    (*dequeue)->capacity = capacity;

    /* destroy */
    free( old_dequeue );
    old_dequeue = NULL;

    return 0;
}

/**
 * Clear all data from the dequeue.
 */
int dequeue_clear( dequeue_t ** dequeue )
{
    int i = 0;

    /* check */
    if( !dequeue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_clear" );

        return -1;
    }

    if( !*dequeue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_clear" );

        return -1;
    }

    assert( (*dequeue)->attr.free ); 

    /* remove items */
    for( i = 0; (*dequeue)->count > 0; ++i )
    {
        (*dequeue)->attr.free( (*dequeue)->dequeue[ i ] );
        (*dequeue)->dequeue[ i ] = NULL;
        (*dequeue)->count--;
    }

    assert( (*dequeue)->count == 0 );

    return 0;
}

/**
 * Get data at the front of the dequeue.
 */
inline void * dequeue_peek_front( const dequeue_t * dequeue )
{
    /* check */
    if( !dequeue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_peek_front" );

        return NULL;
    }

    return dequeue->count ? dequeue->dequeue[ 0 ] : NULL;
}

/**
 * Get data at the back of the dequeue.
 */
inline void * dequeue_peek_back( const dequeue_t * dequeue )
{
    /* check */
    if( !dequeue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_peek_back" );

        return NULL;
    }

    return dequeue->count ? dequeue->dequeue[ dequeue->count - 1 ] : NULL;
}

/**
 * Check if dequeue contains data.
 */
int dequeue_contains( const dequeue_t * dequeue, const void * data )
{
    int i = 0;

    /* check */
    if( !dequeue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_contains" );

        return -1;
    }

    if( !data )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_contains" );

        return -1;
    }

    assert( dequeue->attr.compare );

    /* look for data */
    for( i = 0; i < dequeue->count; ++i )
    {
        if( dequeue->attr.compare( dequeue->dequeue[ i ], data ) == 0 )
        {
            return 1;
        }
    }

    return 0;
}

/**
 * Insert data into the front of the dequeue.
 */
int dequeue_insert_front( dequeue_t ** dequeue, void * data )
{
    int i = 0;

    /* check */
    if( !dequeue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_insert_front" );

        return -1;
    }

    if( !*dequeue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_insert_front" );

        return -1;
    }

    if( !data )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_insert_front" );

        return -1;
    }

    if( (*dequeue)->count >= (*dequeue)->capacity )
    {
        error_code = ERROR_OVERFLOW;
        error_print( "dequeue_insert_front" );

        return -1;
    }

    assert( (*dequeue)->attr.copy );

    /* shift */
    for( i = (*dequeue)->count; i > 0; --i )
    {
        (*dequeue)->dequeue[ i ] = (*dequeue)->dequeue[ i - 1 ];
    }
    (*dequeue)->dequeue[ 0 ] = NULL;

    /* insert */
    (*dequeue)->dequeue[ 0 ] = (*dequeue)->attr.copy( data );
    (*dequeue)->count++;

    assert( (*dequeue)->count <= (*dequeue)->capacity );

    return 0;
}

/**
 * Insert data into the dequeue.
 */
int dequeue_insert_back( dequeue_t ** dequeue, void * data )
{
    /* check */
    if( !dequeue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_insert_back" );

        return -1;
    }

    if( !*dequeue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_insert_back" );

        return -1;
    }

    assert( data );
    if( !data )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_insert_back" );

        return -1;
    }

    if( (*dequeue)->count >= (*dequeue)->capacity )
    {
        error_code = ERROR_OVERFLOW;
        error_print( "dequeue_insert_back" );

        return -1;
    }

    assert( (*dequeue)->attr.copy );

    /* insert */
    (*dequeue)->dequeue[ (*dequeue)->count ] = (*dequeue)->attr.copy( data );
    (*dequeue)->count++;

    assert( (*dequeue)->count <= (*dequeue)->capacity );

    return 0;
}

/**
 * Push data into the front of the dequeue.
 *
 * This function is an alias for dequeue_insert_front.
 */
inline int dequeue_push_front( dequeue_t ** dequeue, void * data )
{
    return dequeue_insert_front( dequeue, data );
}

/**
 * Push data into the back of the dequeue.
 *
 * This function is an alias for dequeue_insert_back.
 */
inline int dequeue_push_back( dequeue_t ** dequeue, void * data )
{
    return dequeue_insert_back( dequeue, data );
}

/**
 * Remove data from the front of the dequeue.
 */
int dequeue_remove_front( dequeue_t ** dequeue )
{
    int i = 0;

    /* check */
    if( !dequeue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_remove_front" );

        return -1;
    }

    if( !*dequeue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_remove_front" );

        return -1;
    }

    if( (*dequeue)->count <= 0 )
    {
        error_code = ERROR_UNDERFLOW;
        error_print( "dequeue_remove_front" );

        return -1;
    }

    assert( (*dequeue)->attr.free );

    /* remove */
    (*dequeue)->attr.free( (*dequeue)->dequeue[  0 ] );
    (*dequeue)->dequeue[ 0 ] = NULL;
    (*dequeue)->count--;

    /* shift */
    for( i = 0; i < (*dequeue)->count; ++i )
    {
        (*dequeue)->dequeue[ i ] = (*dequeue)->dequeue[ i + 1 ];
    }
    (*dequeue)->dequeue[ i ] = NULL;

    assert( (*dequeue)->count >= 0 );

    return 0;
}

/**
 * Remove data from the back of the dequeue.
 */
int dequeue_remove_back( dequeue_t ** dequeue )
{
    /* check */
    if( !dequeue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_remove_back" );

        return -1;
    }

    if( !*dequeue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_remove_back" );

        return -1;
    }

    if( (*dequeue)->count <= 0 )
    {
        error_code = ERROR_UNDERFLOW;
        error_print( "dequeue_remove_back" );

        return -1;
    }

    assert( (*dequeue)->attr.free );

    /* remove */
    (*dequeue)->attr.free( (*dequeue)->dequeue[ (*dequeue)->count - 1 ] );
    (*dequeue)->dequeue[ (*dequeue)->count - 1 ] = NULL;
    (*dequeue)->count--;

    assert( (*dequeue)->count >= 0 );

    return 0;
}

/**
 * Pop data from the front of the dequeue.
 *
 * This function is an alias for dequeue_remove_front.
 */
inline int dequeue_pop_front( dequeue_t ** dequeue )
{
    return dequeue_remove_front( dequeue );
}

/**
 * Pop data from the back of the dequeue.
 *
 * This function is an alias for dequeue_remove_back.
 */
inline int dequeue_pop_back( dequeue_t ** dequeue )
{
    return dequeue_remove_back( dequeue );
}

/**
 * Remove data from the dequeue.
 */
int dequeue_remove_item( dequeue_t ** dequeue, void * data )
{
    int i = 0;

    /* check */
    if( !dequeue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_remove_item" );

        return -1;
    }

    if( !*dequeue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_remove_item" );

        return -1;
    }

    if( !data )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_remove_item" );

        return -1;
    }

    assert( (*dequeue)->attr.compare );
    assert( (*dequeue)->attr.free );

    /* find data */
    for( i = 0; i < (*dequeue)->count; ++i )
    {
        if( (*dequeue)->attr.compare( (*dequeue)->dequeue[ i ], data ) == 0 )
        {
            (*dequeue)->attr.free( (*dequeue)->dequeue[ i ] );
            (*dequeue)->dequeue[ i ] = NULL;

            (*dequeue)->count--;

            /* shift */
            for( ; i < (*dequeue)->count; ++i )
            {
                (*dequeue)->dequeue[ i ] = (*dequeue)->dequeue[ i + 1 ];
            }
            (*dequeue)->dequeue[ i ] = NULL;

            assert( (*dequeue)->count >= 0 );

            return 0;
        }
    }

    error_code = ERROR_NOT_FOUND;
    error_print( "dequeue_remove_item" );
    
    return -1;
}

/**
 * Print a dequeue.
 */
int dequeue_print( const dequeue_t * dequeue, FILE * fp )
{
    int i = 0;
    int nchr = 0;

    /* check */
    if( !dequeue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_print" );

        return -1;
    }

    if( !fp )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_print" );

        return -1;
    }

    assert( dequeue->attr.print );
    
    /* print */
    nchr += fprintf( fp, "Dequeue (%p)\n", (void *)dequeue );
    nchr += fprintf( fp, " - capacity = %d\n", dequeue->capacity );
    nchr += fprintf( fp, " - count = %d\n", dequeue->count );
    nchr += fprintf( fp, " - memory = %d\n", dequeue->memory );

    for( i = 0; i < dequeue->count; ++i )
    {
        nchr += dequeue->attr.print( dequeue->dequeue[ i ], fp );
        nchr += fprintf( fp, "\n" );
    }

    return nchr;
}

/**
 * Get the memory usage of the dequeue.
 */
inline int dequeue_memory( const dequeue_t * dequeue )
{
    /* check */
    if( !dequeue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_memory" );

        return -1;
    }

    assert( dequeue->memory > 0 );

    return dequeue->memory;
}

/**
 * Get the number of items in the dequeue.
 */
inline int dequeue_count( const dequeue_t * dequeue )
{
    /* check */
    if( !dequeue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_count" );

        return -1;
    }

    assert( dequeue->count >= 0 );

    return dequeue->count;
}

/**
 * Get the capacity of the dequeue.
 */
inline int dequeue_capacity( const dequeue_t * dequeue )
{
    /* check */
    if( !dequeue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_capacity" );

        return -1;
    }

    assert( dequeue->capacity > 0 );

    return dequeue->capacity;
}
