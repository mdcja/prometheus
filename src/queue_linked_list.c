/**
 * This file provides an implementation of a queue using a modified linked list
 * data structure.
 *
 * @brief Provide a queue implemented using a linked list.
 * @author Julian Martinez del Campo
 * @license GNU Lesser General Public License (LGPL) https://www.gnu.org/copyleft/lesser.html
 *
 * The memory complexity for this queue is O(n).
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
 * capacity     | ----
 * print        | O(n)
 * clear        | O(n)
 * reserve      | ----
 * remove item  | O(n)
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "queue.h"
#include "error.h"
#include "attributes.h"

/**
 * A linked list node
 */
struct _queue_node {
    void * data;
    struct _queue_node * next;
};

/**
 * A queue with a linked list data structure.
 */
struct _queue_type {
    struct _queue_node * head;
    struct _queue_node * tail;
    int count;
    int memory;
    attr_t attr;
};

/**
 * Create a new queue.
 *
 * This function creates a new queue using a linked list data structure. The
 * capacity is ignored.
 */
queue_t * queue_create( const attr_t * attr, int capacity )
{
    queue_t * new_queue = NULL;

    /* create */
    new_queue = (queue_t *)calloc( 1, sizeof( queue_t ) );
    assert( new_queue );
    if( !new_queue )
    {
        error_code = ERROR_MEMORY_ALLOCATION_FAILED;
        error_print( "queue_create" );

        return NULL;
    }

    /* initialize */
    new_queue->head = NULL;
    new_queue->tail = NULL;
    new_queue->count = 0;
    new_queue->memory = sizeof( queue_t );
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
 * This function deallocates all elements in the queue, then deallocates the
 * queue data structure.
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

    /* deallocate all items in queue */
    if( queue_clear( queue ) != 0 )
    {
        return -1;
    }

    free( *queue );
    *queue = NULL;

    return 0;
}

/**
 * Resize the queue.
 *
 * This function does nothing.
 */
int queue_reserve( queue_t ** queue, int capacity )
{
    error_code = ERROR_NOT_IMPLEMENTED;
    error_print( "queue_reserve" );

    return -1;
}

/**
 * Clear all data from the queue.
 *
 * This function deallocates all items in the linked list.
 */
int queue_clear( queue_t ** queue )
{
    struct _queue_node * current = NULL;
    struct _queue_node * temp = NULL;

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

    /* iterate through queue */
    for( current = (*queue)->head; current != NULL; )
    {
        temp = current;
        current = current->next;

        /* deallocate item */
        (*queue)->attr.free( temp->data );
        temp->data = NULL;

        free( temp );
        temp = NULL;

        (*queue)->count--;
        (*queue)->memory -= sizeof( struct _queue_node );
    }
    (*queue)->head = NULL;
    (*queue)->tail = NULL;

    assert( (*queue)->count == 0 );
    assert( (*queue)->memory == sizeof( queue_t ) );

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

    return queue->count ? queue->head->data : NULL;
}

/**
 * Check if queue contains data.
 *
 * Iterate through the entire linked list and look for data.
 */
int queue_contains( const queue_t * queue, const void * data )
{
    struct _queue_node * current = NULL;

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

    /* iterate through list */
    for( current = queue->head; current != NULL; current = current->next )
    {
        if( queue->attr.compare( current->data, data ) == 0 )
        {
            return 1;
        }
    }

    return 0;
}

/**
 * Insert data into the queue.
 *
 * Insert an item at the end of the list.
 */
int queue_insert( queue_t ** queue, void * data )
{
    struct _queue_node * new_node = NULL;

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

    assert( (*queue)->attr.copy );

    /* create */
    new_node = (struct _queue_node *)calloc( 1, sizeof( struct _queue_node ) );
    assert( new_node );
    if( !new_node )
    {
        error_code = ERROR_MEMORY_ALLOCATION_FAILED;
        error_print( "queue_insert" );

        return -1;
    }

    new_node->data = (*queue)->attr.copy( data );
    new_node->next = NULL;

    /* insert node */
    if( (*queue)->count == 0 )
    {
        (*queue)->head = new_node;
        (*queue)->tail = new_node;
    }
    else
    {
        (*queue)->tail->next = new_node;
        (*queue)->tail = new_node;
    }

    (*queue)->count++;
    (*queue)->memory += sizeof( struct _queue_node );

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
 *
 * Remove node at the beginning of the list.
 */
int queue_remove( queue_t ** queue )
{
    struct _queue_node * temp = NULL;

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

    /* remove node */
    temp = (*queue)->head;

    if( (*queue)->count == 1 )
    {
        (*queue)->head = NULL;
        (*queue)->tail = NULL;
    }
    else
    {
        (*queue)->head = (*queue)->head->next;
    }

    (*queue)->count--;
    (*queue)->memory -= sizeof( struct _queue_node );

    /* deallocate node */
    (*queue)->attr.free( temp->data );
    temp->data = NULL;

    free( temp );
    temp = NULL;

    assert( (*queue)->count >= 0 );
    assert( (*queue)->memory >= sizeof( queue_t ) );

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
 */
int queue_remove_item( queue_t ** queue, void * data )
{
    struct _queue_node * current = NULL;
    struct _queue_node * prev = NULL;
    struct _queue_node * temp = NULL;

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

    /* find item */
    for( current = (*queue)->head, prev = NULL; current != NULL; )
    {
        if( (*queue)->attr.compare( current->data, data ) == 0 )
        {
            /* remove item */
            temp = current;

            if( current == (*queue)->head )
            {
                (*queue)->head = current->next;
            }
            else if( current == (*queue)->tail )
            {
                (*queue)->tail = prev;
            }
            else
            {
                prev->next = current->next;
            }
            current = current->next;

            /* deallocate node */
            (*queue)->attr.free( temp->data );
            temp->data = NULL;

            free( temp );
            temp = NULL;

            (*queue)->count--;
            (*queue)->memory -= sizeof( struct _queue_node );

            assert( (*queue)->count >= 0 );
            assert( (*queue)->memory >= sizeof( queue_t ) );

            return 0;
        }
        else
        {
            prev = current;
            current = current->next;
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
    struct _queue_node * current = NULL;
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
    nchr += fprintf( fp, " - count = %d\n", queue->count );
    nchr += fprintf( fp, " - memory = %d\n", queue->memory );

    for( current = queue->head; current != NULL; current = current->next )
    {
        nchr += queue->attr.print( current->data, fp );
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

    assert( queue->memory >= sizeof( queue_t ) );

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
 *
 * This function does nothing.
 */
int queue_capacity( const queue_t * queue )
{
    error_code = ERROR_NOT_IMPLEMENTED;
    error_print( "queue_capacity" );

    return -1;
}
