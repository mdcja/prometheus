/* Author: Julian Martinez del Campo
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#ifndef _EMBEDDED
#include <string.h>
#endif /* _EMBEDDED */

#include "queue.h"
#include "error.h"
#include "attributes.h"

/* A linked list node
 */
struct _queue_node {
    void * data;
    struct _queue_node * next;
};

/* A queue with a linked list data structure.
 */
struct _queue_type {
    struct _queue_node * head;  /* first item in queue */
    struct _queue_node * tail;  /* last item in queue */
    int count;                  /* number of items in queue */
    int memory;                 /* memory used by queue */
    attr_t attr;                /* data attributes */
};

/* Create a queue, capacity is ignored.
 */
queue_t * queue_create( const attr_t * attr, int capacity )
{
    queue_t * new_queue = NULL;

    /* create */
#ifndef _EMBEDDED
    new_queue = (queue_t *)calloc( 1, sizeof( queue_t ) );
#else
    new_queue = (queue_t *)malloc( sizeof( queue_t ) );
    memset( new_queue, 0, sizeof( queue_t ) );
#endif /* _EMBEDDED */
    if( new_queue == NULL )
    {
        error_code = E_MEMORY_ALLOCATION_FAILED;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "queue_create: new_queue" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

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

#ifndef _EMBEDDED
    assert( new_queue->attr.copy );
    assert( new_queue->attr.free );
    assert( new_queue->attr.compare );
    assert( new_queue->attr.print );
#endif /* _EMBEDDED */

    return new_queue;
}

int queue_destroy( queue_t ** queue )
{
    /* check */
    if( queue == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "queue_destroy: queue" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    if( *queue == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "queue_destroy: *queue" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

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

/* Resize a queue, the queue is dynamically allocated so ignore resizes, and
 * return 0.
 */
int queue_reserve( queue_t ** queue, int capacity )
{
    /* check */
    if( queue == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "queue_reserve: queue" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    if( *queue == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "queue_reserve: *queue" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    return 0;
}

int queue_clear( queue_t ** queue )
{
    struct _queue_node * current = NULL;
    struct _queue_node * temp = NULL;

    /* check */
    if( queue == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "queue_clear: queue" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    if( *queue == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "queue_clear: *queue" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

#ifndef _EMBEDDED
    assert( (*queue)->attr.free );
#endif /* _EMBEDDED */

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

#ifndef _EMBEDDED
    assert( (*queue)->count == 0 );
    assert( (*queue)->memory == sizeof( queue_t ) );
#endif /* _EMBEDDED */

    return 0;
}

void * queue_peek( const queue_t * queue )
{
    /* check */
    if( queue == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "queue_peek: queue" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return NULL;
    }

    return queue->count ? queue->head->data : NULL;
}

int queue_contains( const queue_t * queue, const void * data )
{
    struct _queue_node * current = NULL;

    /* check */
    if( queue == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "queue_contains: queue" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    if( data == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "queue_contains: data" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

#ifndef _EMBEDDED
    assert( queue->attr.compare );
#endif /* _EMBEDDED */

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

int queue_insert( queue_t ** queue, void * data )
{
    struct _queue_node * new_node = NULL;

    /* check */
    if( queue == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "queue_insert: queue" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    if( *queue == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "queue_insert: *queue" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    if( data == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "queue_insert: data" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

#ifndef _EMBEDDED
    assert( (*queue)->attr.copy );
#endif /* _EMBEDDED */

    /* create */
    
#ifndef _EMBEDDED
    new_node = (struct _queue_node *)calloc( 1, sizeof( struct _queue_node ) );
#else
    new_node = (struct _queue_node *)malloc( sizeof( struct _queue_node ) );
    memset( new_node, 0, sizeof( struct _queue_node ) );
#endif /* _EMBEDDED */
    if( new_node == NULL )
    {
        error_code = E_MEMORY_ALLOCATION_FAILED;

#ifndef _EMBEDDED
        error_print( "queue_insert: new_node" );
#endif /* _EMBEDDED */

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

int queue_remove( queue_t ** queue )
{
    struct _queue_node * temp = NULL;

    /* check */
    if( queue == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "queue_remove: queue" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    if( *queue == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "queue_remove: *queue" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    if( (*queue)->count <= 0 )
    {
        error_code = E_UNDERFLOW;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "queue_remove: count" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

#ifndef _EMBEDDED
    assert( (*queue)->attr.free );
#endif /* _EMBEDDED */

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

#ifndef _EMBEDDED
    assert( (*queue)->count >= 0 );
    assert( (*queue)->memory >= sizeof( queue_t ) );
#endif /* _EMBEDDED */

    return 0;
}

int queue_remove_item( queue_t ** queue, void * data )
{
    struct _queue_node * current = NULL;
    struct _queue_node * prev = NULL;
    struct _queue_node * temp = NULL;

    /* check */
    if( queue == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "queue_remove_item: queue" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    if( *queue == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "queue_remove_item: *queue" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    if( data == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "queue_remove_item: data" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

#ifndef _EMBEDDED
    assert( (*queue)->attr.compare );
    assert( (*queue)->attr.free );
#endif /* _EMBEDDED */

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

#ifndef _EMBEDDED
            assert( (*queue)->count >= 0 );
            assert( (*queue)->memory >= sizeof( queue_t ) );
#endif /* _EMBEDDED */

            return 0;
        }
        else
        {
            prev = current;
            current = current->next;
        }
    }

    error_code = E_NOT_FOUND;

#ifndef _EMBEDDED
#if _VERBOSE > 0
    error_print( "queue_remove_item" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

    return -1;
}

#ifndef _EMBEDDED
int queue_print( const queue_t * queue, FILE * fp )
{
    struct _queue_node * current = NULL;
    int nchr = 0;

    /* check */
    if( queue == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "queue_print: queue" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    if( fp == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "queue_print: fp" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

#ifndef _EMBEDDED
    assert( queue->attr.print );
#endif /* _EMBEDDED */

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
#endif /* _EMBEDDED */

int queue_memory( const queue_t * queue )
{
    /* check */
    if( queue == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "queue_memory: queue" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

#ifndef _EMBEDDED
    assert( queue->memory >= sizeof( queue_t ) );
#endif /* _EMBEDDED */

    return queue->memory;
}

int queue_count( const queue_t * queue )
{
    /* check */
    if( queue == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "queue_count: queue" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

#ifndef _EMBEDDED
    assert( queue->count >= 0 );
#endif /* _EMBEDDED */

    return queue->count;
}

/* Always return the number of items + 1. This way the capacity of the queue is
 * always greater than the size of the queue.
 */
int queue_capacity( const queue_t * queue )
{
    /* check */
    if( queue == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "queue_capacity: queue" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

#ifndef _EMBEDDED
    assert( queue->count >= 0 );
#endif /* _EMBEDDED */

    return queue->count + 1;
}
