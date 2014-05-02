/**
 * This file provides an implementation of a queue using an array data
 * structure.
 *
 * \brief Provide a queue implemented using an array.
 * \author Julian Martinez del Campo
 * \license GNU Lesser General Public License (LGPL) https://www.gnu.org/copyleft/lesser.html
 *
 * The memory complexity for this queue is O(1).
 * 
 * This implementation provides the following run time complexity for a queue
 * with n elements:
 *
 * Operation    | Efficiency
 * -------------|-----------
 * create       | O(1)
 * destroy      | O(n)
 * contains     | O(n)
 * peek         | O(1)
 * insert/push  | O(1)
 * remove/pop   | O(n)
 * count        | O(1)
 * memory       | O(1)
 * capacity     | O(1)
 * print        | O(n)
 * clear        | O(n)
 * reserve      | O(n)
 * remove item  | O(n)
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "queue.h"
#include "error.h"
#include "attributes.h"

/**
 * A queue using an array.
 */
struct _queue_type {
    void ** queue;
    int capacity;
    int count;
    int memory;
    attr_t attr;
};

/**
 * Create a new queue.
 *
 * Creates a new queue that uses an array.
 */
queue_t * queue_create( const attr_t * attr, int capacity )
{
    queue_t * new_queue = NULL;

    /* check */
    if( capacity <= 0 )
    {
        error_code = ERROR_INVALID_CAPACITY;
        error_print( "queue_create" );

        return NULL;
    }

    /* create queue */
    new_queue = (queue_t *)calloc( 1, sizeof( queue_t ) );
    assert( new_queue );
    if( !new_queue )
    {
        error_code = ERROR_MEMORY_ALLOCATION_FAILED;
        error_print( "queue_create" );

        return NULL;
    }

    new_queue->queue = (void **)calloc( capacity, sizeof( void * ) );
    assert( new_queue->queue );
    if( !new_queue->queue )
    {
        error_code = ERROR_MEMORY_ALLOCATION_FAILED;
        error_print( "queue_create" );

        return NULL;
    }

    /* initialize */
    new_queue->count = 0;
    new_queue->capacity = capacity;
    new_queue->memory = sizeof( queue_t ) + capacity * sizeof( void * );
    if( attr )
    {
        new_queue->attr = *attr; 
    }
    else
    {
        attr_init( &new_queue->attr );
    }

    assert( new_queue->attr.copy );
    assert( new_queue->attr.free );
    assert( new_queue->attr.compare );
    assert( new_queue->attr.print );

    return new_queue;
}

/**
 * Destroy a queue.
 *
 * This function first deallocates all items contained in the queue, then
 * deallocates the queue.
 */
int queue_destroy( queue_t ** queue )
{
    /* check */
    if( !queue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "queue_destroy" );

        return -1;
    }

    if( !*queue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "queue_destroy" );

        return -1;
    }

    /* clear items */
    if( queue_clear( queue ) != 0 )
    {
        return -1;
    }

    /* deallocate queue */
    free( (*queue)->queue );
    (*queue)->queue = NULL;

    free( *queue );
    *queue = NULL;

    return 0;
}

/**
 * Resize the queue.
 *
 * This function creates a new array of the desired capacity, copies the
 * existing array over and replaces the array.
 */
int queue_reserve( queue_t ** queue, int capacity )
{
    void ** new_queue = NULL;
    void ** old_queue = NULL;
    int i = 0;

    /* check */
    if( !queue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "queue_reserve" );

        return -1;
    }

    if( !*queue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "queue_reserve" );

        return -1;
    }

    if( capacity <= (*queue)->count )
    {
        error_code = ERROR_INVALID_CAPACITY;
        error_print( "queue_reserve" );

        return -1;
    }

    /* create */
    new_queue = (void **)calloc( capacity, sizeof( void * ) );
    assert( new_queue );
    if( !new_queue )
    {
        error_code = ERROR_MEMORY_ALLOCATION_FAILED;
        error_print( "queue_reserve" );
    }

    /* copy */
    for( i = 0; i < (*queue)->count; ++i )
    {
        new_queue[i] = (*queue)->queue[i];
    }

    /* switch */
    old_queue = (*queue)->queue;
    (*queue)->capacity = capacity;
    (*queue)->queue = new_queue;
    (*queue)->memory = sizeof( queue_t ) + capacity * sizeof( void * );

    /* destroy */
    free( old_queue );
    old_queue = NULL;

    return 0;
}

/**
 * Clear all data from the queue.
 *
 * Deallocate all the data contained in the queue.
 */
int queue_clear( queue_t ** queue )
{
    int i = 0;

    /* check */
    if( !queue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "queue_clear" );

        return -1;
    }

    if( !*queue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "queue_clear" );

        return -1;
    }

    assert( (*queue)->attr.free );

    /* clear */
    for( i = 0; (*queue)->count > 0; ++i )
    {
        (*queue)->attr.free( (*queue)->queue[ i ] );
        (*queue)->queue[ i ] = NULL;
        (*queue)->count--;
    }
 
    assert( (*queue)->count == 0 );

    return 0;
}

/**
 * Get data at the front of the queue.
 */
inline void * queue_peek( const queue_t * queue )
{
    /* check */
    if( !queue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "queue_peek" );

        return NULL;
    }

    return queue->count ? queue->queue[ 0 ] : NULL;
}

/**
 * Check if queue contains data.
 *
 * Each item in the queue is checked against the data using the compare()
 * function in the attributes.
 */
int queue_contains( const queue_t * queue, const void * data )
{
    int i = 0;

    /* check */
    if( !queue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "queue_contains" );

        return -1;
    }

    assert( queue->attr.compare );

    /* search for item */
    for( i = 0; i < queue->count; ++i )
    {
        if( queue->attr.compare( queue->queue[ i ], data ) == 0 )
        {
            return 1;
        }
    }
    
    return 0;
}

/**
 * Insert data into the queue.
 */
int queue_insert( queue_t ** queue, void * data )
{
    /* check */
    if( !queue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "queue_insert" );

        return -1;
    }

    if( !*queue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "queue_insert" );

        return -1;
    }

    if( !data )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "queue_insert" );

        return -1;
    }

    if( (*queue)->count >= (*queue)->capacity )
    {
        error_code = ERROR_OVERFLOW;
        error_print( "queue_insert" );

        return -1;
    }

    assert( (*queue)->attr.copy );

    /* insert */
    (*queue)->queue[ (*queue)->count ] = (*queue)->attr.copy( data );
    (*queue)->count++;

    assert( (*queue)->count <= (*queue)->capacity );

    return 0;
}

/**
 * Push data into the queue.
 */
inline int queue_push( queue_t ** queue, void * data )
{
    return queue_insert( queue, data );
}

/**
 * Remove data from the queue.
 *
 * Removes data from the front of the queue if there are items in the queue.
 */
int queue_remove( queue_t ** queue )
{
    int i = 0;

    /* check */
    if( !queue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "queue_remove" );

        return -1;
    }

    if( !*queue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "queue_remove" );

        return -1;
    }

    if( (*queue)->count <= 0 )
    {
        error_code = ERROR_UNDERFLOW;
        error_print( "queue_remove" );

        return -1;
    }

    assert( (*queue)->attr.free );

    /* remove */
    (*queue)->attr.free( (*queue)->queue[ 0 ] );
    (*queue)->queue[ 0 ] = NULL;
    (*queue)->count--;

    /* shift */
    for( i = 0; i < (*queue)->count; ++i )
    {
        (*queue)->queue[ i ] = (*queue)->queue[ i + 1 ];
    }
    (*queue)->queue[ i ] = NULL;

    assert( (*queue)->count >= 0 );

    return 0;
}

/**
 * Pop data from the queue.
 */
inline int queue_pop( queue_t ** queue )
{
    return queue_remove( queue );
}

/**
 * Remove data from the queue.
 *
 * Check each item in the queue and look for the given data. If found,
 * deallocate the data, and shift array over.
 */
int queue_remove_item( queue_t ** queue, void * data )
{
    int i = 0;

    /* check */
    if( !queue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "queue_remove_item" );

        return -1;
    }

    if( !*queue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "queue_remove_item" );

        return -1;
    }

    if( !data )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "queue_remove_item" );

        return -1;
    }

    assert( (*queue)->attr.compare );
    assert( (*queue)->attr.free );

    /* find data */
    for( i = 0; i < (*queue)->count; ++i )
    {
        if( (*queue)->attr.compare( (*queue)->queue[ i ], data ) == 0 )
        {
            (*queue)->attr.free( (*queue)->queue[ i ] );
            (*queue)->queue[ i ] = NULL;
            (*queue)->count--;

            /* shift */
            for( ; i < (*queue)->count; ++i )
            {
                (*queue)->queue[ i ] = (*queue)->queue[ i + 1 ];
            }
            (*queue)->queue[ i ] = NULL;

            assert( (*queue)->count >= 0 );

            return 0;
        }
    }

    error_code = ERROR_NOT_FOUND;
    error_print( "queue_remove_item" );

    return -1;
}

/**
 * Print a queue.
 *
 * Prints information about the queue, such as the queue location, its
 * capacity, count, memory usage, and all data contained within.
 */
int queue_print( const queue_t * queue, FILE * fp )
{
    int i = 0;
    int nchr = 0;

    /* check */
    if( !queue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "queue_print" );

        return -1;
    }

    if( !fp )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "queue_print" );

        return -1;
    }

    assert( queue->attr.print );

    /* print */
    nchr += fprintf( fp, "Queue (%p)\n", (void *)queue );
    nchr += fprintf( fp, " - capacity = %d\n", queue->capacity );
    nchr += fprintf( fp, " - count = %d\n", queue->count );
    nchr += fprintf( fp, " - memory = %d\n", queue->memory );

    for( i = 0; i < queue->count; ++i )
    {
        nchr += queue->attr.print( queue->queue[ i ], fp );
        nchr += fprintf( fp, "\n" );
    }

    return nchr;
}

/**
 * Get the memory usage of the queue.
 */
inline int queue_memory( const queue_t * queue )
{
    /* check */
    if( !queue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "queue_memory" );

        return -1;
    }

    assert( queue->memory > 0 );

    return queue->memory;
}

/**
 * Get the number of items in the queue.
 */
inline int queue_count( const queue_t * queue )
{
    /* check */
    if( !queue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "queue_count" );

        return -1;
    }

    assert( queue->count >= 0 );

    return queue->count;
}

/**
 * Get the capacity of the queue.
 */
inline int queue_capacity( const queue_t * queue )
{
    /* check */
    if( !queue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "queue_capacity" );

        return -1;
    }

    assert( queue->capacity > 0 );

    return queue->capacity;
}
