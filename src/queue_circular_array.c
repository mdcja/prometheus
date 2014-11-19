/* Author: Julian Martinez del Campo
 *
 * This file implements queue.h using a circular array.
 *
 * If _EMBEDDED macro is defined then all functions depending on stdio.h are
 * disabled. This may be useful when programming on embedded systems.
 *
 * If _VERBOSE > 0 macro is defined then error messages will be printed by the
 * functions.
 */
#include <stdlib.h>
#ifndef _EMBEDDED
#include <stdio.h>
#include <assert.h>
#else
#include <string.h>
#endif /* _EMBEDDED */

#include "queue.h"
#include "error.h"
#include "attributes.h"

/* A queue with a circular buffer data structure
 */
struct _queue_type {
    void ** queue;  /* the actual queue */
    int head;       /* index of first item */
    int tail;       /* index of the last item */
    int capacity;   
    int count;      /* the number of items in the queue */
    int memory;     /* memory used by the queue */
    attr_t attr;    /* data attributes */
};

queue_t * queue_create( const attr_t * attr, int capacity )
{
    queue_t * new_queue = NULL;

    /* check */
    if( capacity < 1 )
    {
        error_code = E_INVALID_CAPACITY;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "queue_create: capacity" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return NULL;
    }

    /* create */
    new_queue = (queue_t *)calloc( 1, sizeof( queue_t ) );
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
    
#ifndef _EMBEDDED
    new_queue->queue = (void **)calloc( capacity, sizeof( void * ) );
#else
    new_queue->queue = (void **)malloc( capacity * sizeof( void * ) );
    memset( new_queue->queue, 0, capacity * sizeof( void * ) );
#endif /* _EMBEDDED */
    if( new_queue->queue == NULL )
    {
        error_code = E_MEMORY_ALLOCATION_FAILED;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "queue_create: new_queue->queue" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

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

int queue_reserve( queue_t ** queue, int capacity )
{
    void ** new_queue = NULL;
    void ** old_queue = NULL;
    int i = 0;
    int j = 0;

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

    if( capacity < (*queue)->count )
    {
        error_code = E_INVALID_CAPACITY;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "queue_reserve: capacity" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    /* create */
    new_queue = (void **)calloc( capacity, sizeof( void * ) );
    if( !new_queue )
    {
        error_code = E_MEMORY_ALLOCATION_FAILED;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "queue_reserve: new_queue" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

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

int queue_clear( queue_t ** queue )
{
    int i = 0;

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

    /* clear all items */
    for( i = (*queue)->head; (*queue)->count > 0; ++i )
    {
        (*queue)->attr.free( (*queue)->queue[ i % (*queue)->capacity ] );
        (*queue)->queue[ i % (*queue)->capacity ] = NULL;

        (*queue)->count--;
    }
    (*queue)->head = 0;
    (*queue)->tail = 0;

#ifndef _EMBEDDED
    assert( (*queue)->count == 0 );
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

    return queue->count ? queue->queue[ queue->head ] : NULL;
}

int queue_contains( const queue_t * queue, const void * data )
{
    int i = 0;
    int j = 0;

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

int queue_insert( queue_t ** queue, void * data )
{
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

    if( (*queue)->count >= (*queue)->capacity )
    {
        error_code = E_OVERFLOW;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "queue_insert: count" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

#ifndef _EMBEDDED
    assert( (*queue)->attr.copy );
#endif /* _EMBEDDED */

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

#ifndef _EMBEDDED
    assert( (*queue)->count <= (*queue)->capacity );
#endif /* _EMBEDDED */

    return 0;
}

int queue_remove( queue_t ** queue )
{
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

    /* remove */
    (*queue)->attr.free( (*queue)->queue[ (*queue)->head ] );
    (*queue)->queue[ (*queue)->head ] = NULL;

    (*queue)->head = ((*queue)->head + 1) % (*queue)->capacity;
    (*queue)->count--;

#ifndef _EMBEDDED
    assert( (*queue)->count >= 0 );
#endif /* _EMBEDDED */

    return 0;
}

int queue_remove_item( queue_t ** queue, void * data )
{
    int i = 0;
    int j = 0;

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

#ifndef _EMBEDDED
            assert( (*queue)->count >= 0 );
#endif /* _EMBEDDED */

            return 0;
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
    int i = 0;
    int j = 0;
    int nchr = 0;

    /* check */
    if( queue == NULL )
    {
        error_code = E_NULL_PARAMETER;

#if _VERBOSE > 0
        error_print( "queue_print: queue" );
#endif /* _VERBOSE */

        return -1;
    }

    if( fp == NULL )
    {
        error_code = E_NULL_PARAMETER;

#if _VERBOSE > 0
        error_print( "queue_print: fp" );
#endif /* _VERBOSE */

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
    assert( queue->memory > 0 );
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
    assert( queue->capacity > 0 );
#endif /* _EMBEDDED */

    return queue->capacity;
}
