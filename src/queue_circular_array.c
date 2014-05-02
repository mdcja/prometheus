/**
 * This file provides an implementation of a queue using a circular array data
 * structure.
 *
 * \brief Provide a queue implemented using a circular array.
 * \author Julian Martinez del Campo
 * \license GNU Lesser General Public License (LGPL) https://www.gnu.org/copyleft/lesser.html
 *
 * The memory complexity of this queue is O(1).
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
 * remove/pop   | O(1)
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
 * A queue with a circular buffer data structure
 */
struct _queue_type {
    void ** queue;
    int head;
    int tail;
    int capacity;
    int count;
    int memory;
    attr_t attr;
};

/**
 * Create a new queue.
 */
queue_t * queue_create( const attr_t * attr, int capacity )
{
    queue_t * new_queue = NULL;

    /* check */
    if( capacity < 1 )
    {
        error_code = ERROR_INVALID_CAPACITY;
        error_print( "queue_create" );

        return NULL;
    }

    /* create */
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
    if( !new_queue )
    {
        error_code = ERROR_MEMORY_ALLOCATION_FAILED;
        error_print( "queue_create" );

        return NULL;
    }

    /* initialize */
    new_queue->head = 0;
    new_queue->tail = 0;
    new_queue->capacity = capacity;
    new_queue->count = 0;
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
 * Deallocate all items in the queue, then deallocate the queue itself.
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

    /* deallocate all items */
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
 * Create a new array of the desired capacity, then copy items over and
 * deallocate the old array.
 */
int queue_reserve( queue_t ** queue, int capacity )
{
    void ** new_queue = NULL;
    void ** old_queue = NULL;
    int i = 0;
    int j = 0;

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

    if( capacity < (*queue)->count )
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

        return -1;
    }

    /* copy */
    for( i = (*queue)->head, j = 0; j < (*queue)->count; ++i, ++j )
    {
        new_queue[ j ] = (*queue)->queue[ i % (*queue)->capacity ];
    }

    /* replace */
    old_queue = (*queue)->queue;
    (*queue)->queue = new_queue;
    (*queue)->capacity = capacity;
    (*queue)->head = 0;
    (*queue)->tail = (*queue)->count - 1;
    (*queue)->memory = sizeof( queue_t ) + capacity * sizeof( void * );

    /* deallocate */
    free( old_queue );
    old_queue = NULL;

    return 0;
}

/**
 * Clear all data from the queue.
 *
 * Deallocate all items in the queue.
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

    /* clear all items */
    for( i = (*queue)->head; (*queue)->count > 0; ++i )
    {
        (*queue)->attr.free( (*queue)->queue[ i % (*queue)->capacity ] );
        (*queue)->queue[ i % (*queue)->capacity ] = NULL;

        (*queue)->count--;
    }
    (*queue)->head = 0;
    (*queue)->tail = 0;

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

    return queue->count ? queue->queue[ queue->head ] : NULL;
}

/**
 * Check if queue contains data.
 *
 * Go through all items in the queue and compare against given data.
 */
int queue_contains( const queue_t * queue, const void * data )
{
    int i = 0;
    int j = 0;

    /* check */
    if( !queue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "queue_contains" );

        return -1;
    }

    if( !data )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "queue_contains" );

        return -1;
    }

    assert( queue->attr.compare );

    /* look for data */
    for( i = queue->head, j = 0; j < queue->count; ++i, ++j )
    {
        if( queue->attr.compare( queue->queue[ i % queue->capacity ], data ) == 0 )
        {
            return 1;
        }
    }

    return 0;
}

/**
 * Insert data into the queue.
 *
 * Insert an item at the front of the queue.
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

    /* insert item */
    if( (*queue)->count == 0 )
    {
        (*queue)->queue[ (*queue)->head ] = (*queue)->attr.copy( data );
        (*queue)->tail = (*queue)->head;
    }
    else
    {
        (*queue)->queue[ ( (*queue)->tail + 1) % (*queue)->capacity ] = (*queue)->attr.copy( data );
        (*queue)->tail = ((*queue)->tail + 1) % (*queue)->capacity;
    }
    (*queue)->count++;

    assert( (*queue)->count <= (*queue)->capacity );

    return 0;
}

/**
 * Push data into the queue.
 *
 * This function is an alias for queue_insert.
 */
inline int queue_push( queue_t ** queue, void * data )
{
    return queue_insert( queue, data );
}

/**
 * Remove data from the queue.
 */
int queue_remove( queue_t ** queue )
{
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
    (*queue)->attr.free( (*queue)->queue[ (*queue)->head ] );
    (*queue)->queue[ (*queue)->head ] = NULL;

    (*queue)->head = ((*queue)->head + 1) % (*queue)->capacity;
    (*queue)->count--;

    assert( (*queue)->count >= 0 );

    return 0;
}

/**
 * Pop data from the queue.
 *
 * This function is an alias for queue_remove.
 */
inline int queue_pop( queue_t ** queue )
{
    return queue_remove( queue );
}

/**
 * Remove data from the queue.
 *
 * Look for the item in the queue and then deallocate it.
 */
int queue_remove_item( queue_t ** queue, void * data )
{
    int i = 0;
    int j = 0;

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

    /* look for data */
    for( i = (*queue)->head, j = 0; j < (*queue)->count; ++i, ++j )
    {
        if( (*queue)->attr.compare( (*queue)->queue[ i % (*queue)->capacity ], data ) == 0 )
        {
            (*queue)->attr.free( (*queue)->queue[ i % (*queue)->capacity ] );
            (*queue)->queue[ i % (*queue)->capacity ] = NULL;
            (*queue)->count--;

            /* shift */
            for( ; j < (*queue)->count; ++i, ++j )
            {
                (*queue)->queue[ i % (*queue)->capacity ] = (*queue)->queue[ (i + 1) % (*queue)->capacity ];
            }
            (*queue)->queue[ i ] = NULL;

            if( (*queue)->tail == 0 )
            {
                (*queue)->tail = (*queue)->capacity - 1;
            }
            else
            {
                (*queue)->tail--;
            }

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
 */
int queue_print( const queue_t * queue, FILE * fp )
{
    int i = 0;
    int j = 0;
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

    for( i = queue->head; j < queue->count; ++i, ++j )
    {
        nchr += queue->attr.print( queue->queue[ i % queue->capacity ], fp );
        nchr += fprintf( fp, "\n" );
    }

    return nchr;
}

/**
 * Get the memory usage of the queue.
 */
int queue_memory( const queue_t * queue )
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
int queue_count( const queue_t * queue )
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
int queue_capacity( const queue_t * queue )
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
