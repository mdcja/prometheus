/**
 * This file provides an implementation of a dequeue using a doubly linked list
 * data structure.
 * 
 * \brief Provide a dequeue implemented using a doubly linked list.
 * \author Julian Martinez del Campo
 * \license GNU Lesser General Public License (LGPL) https://www.gnu.org/copyleft/lesser.html
 *
 * This implementation provides the following run time complexity for a dequeue
 * with n elements:
 *
 * Operation            | Complexity
 * ---------------------|-----------
 * create               | O(1)
 * destroy              | O(n)
 * reserve              | ----
 * clear                | O(n)
 * peek_front           | O(1)
 * peek_back            | O(1)
 * contains             | O(n)
 * insert(push)_front   | O(1)
 * insert(push)_back    | O(1)
 * remove(pop)_front    | O(1)
 * remove(pop)_back     | O(1)
 * remove_item          | O(n)
 * print                | O(n)
 * memory               | O(1)
 * capacity             | ----
 * count                | O(1)
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "dequeue.h"
#include "error.h"
#include "attributes.h"

/**
 * a doubly linked list node 
 */
struct _dequeue_node {
    void * data; struct _dequeue_node * prev;
    struct _dequeue_node * next;
};

/**
 * A dequeue data structure
 */
struct _dequeue_type {
    struct _dequeue_node * head;
    struct _dequeue_node * tail;
    int count;
    int memory;
    attr_t attr;
};

/**
 * Create a new dequeue.
 *
 * Create a new dequeue using a doubly linked list, ignore capacity.
 */
dequeue_t * dequeue_create( const attr_t * attr, int capacity )
{
    dequeue_t * new_dequeue = NULL;

    /* create */
    new_dequeue = (dequeue_t *)calloc(1, sizeof( dequeue_t ) );
    assert( new_dequeue );
    if( !new_dequeue )
    {
        error_code = ERROR_MEMORY_ALLOCATION_FAILED;
        error_print( "dequeue_create" );

        return NULL;
    }

    /* init */
    new_dequeue->head = NULL;
    new_dequeue->tail = NULL;
    new_dequeue->count = 0;
    new_dequeue->memory = sizeof( dequeue_t );
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
 *
 * Deallocate items in the dequeue then deallocat the dequeue itself.
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

    /* clear queue */
    if( dequeue_clear( dequeue ) != 0 )
    {
        return -1;
    }

    /* deallocate */
    free( *dequeue );
    *dequeue = NULL;

    return 0;
}

/**
 * Resize the dequeue.
 *
 * This function does nothing, as it does not make sense for a linked list.
 */
inline int dequeue_reserve( dequeue_t ** dequeue, int capacity )
{
    /* error */
    error_code = ERROR_NOT_IMPLEMENTED;
    error_print( "dequeue_reserve" );

    return -1;
}

/**
 * Clear all data from the dequeue.
 */
int dequeue_clear( dequeue_t ** dequeue )
{
    struct _dequeue_node * current = NULL;
    struct _dequeue_node * temp = NULL;

    /* check */
    if( !dequeue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_clear" );

        return -1;
    }

    if( !*dequeue )
    {
        error_code = ERROR_NOT_IMPLEMENTED;
        error_print( "dequeue_clear" );

        return -1;
    }

    assert( (*dequeue)->attr.free );

    /* clear */
    for( current = (*dequeue)->head; current != NULL; )
    {
        temp = current;
        current = current->next;

        /* deallocate node */
        (*dequeue)->attr.free( temp->data );
        temp->data = NULL;

        free( temp );
        temp = NULL;

        (*dequeue)->memory -= sizeof( struct _dequeue_node );
        (*dequeue)->count--;
    }

    (*dequeue)->head = NULL;
    (*dequeue)->tail = NULL;

    assert( (*dequeue)->count == 0 );
    assert( (*dequeue)->memory == sizeof(dequeue_t) );

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

    /* get front */
    return dequeue->head ? dequeue->head->data : NULL;
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

    /* get back */
    return dequeue->tail ? dequeue->tail->data : NULL;
}

/**
 * Check if dequeue contains data.
 *
 * Check each element in the dequeue and compare it against data.
 */
int dequeue_contains( const dequeue_t * dequeue, const void * data )
{
    struct _dequeue_node * current = NULL;

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
    for( current = dequeue->head; current != NULL; current = current->next )
    {
        if( dequeue->attr.compare( current->data, data ) == 0 )
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
    struct _dequeue_node * new_node = NULL;

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

    assert( (*dequeue)->attr.copy );
    
    /* create */
    new_node = (struct _dequeue_node *)calloc( 1, sizeof( struct _dequeue_node ) );
    assert( new_node );
    if( !new_node )
    {
        error_code = ERROR_MEMORY_ALLOCATION_FAILED;
        error_print( "dequeue_insert_front" );

        return -1;
    }
    
    /* init */
    new_node->data = (*dequeue)->attr.copy( data ); 
    new_node->next = NULL;
    new_node->prev = NULL;

    /* insert */
    if( (*dequeue)->count == 0 )
    {
        (*dequeue)->head = new_node;
        (*dequeue)->tail = new_node;
    }
    else
    {
        (*dequeue)->head->prev = new_node;
        new_node->next = (*dequeue)->head;
        (*dequeue)->head = new_node;
    }
    (*dequeue)->count++;
    (*dequeue)->memory += sizeof( struct _dequeue_node );

    return 0;
}

/**
 * Insert data into the dequeue.
 */
int dequeue_insert_back( dequeue_t ** dequeue, void * data )
{
    struct _dequeue_node * new_node = NULL;

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

    if( !data )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_insert_back" );

        return -1;
    }

    assert( (*dequeue)->attr.copy );
    
    /* create */
    new_node = (struct _dequeue_node *)calloc( 1, sizeof( struct _dequeue_node ) );
    assert( new_node );
    if( !new_node )
    {
        error_code = ERROR_MEMORY_ALLOCATION_FAILED;
        error_print( "dequeue_insert_back" );

        return -1;
    }
    
    /* init */
    new_node->data = (*dequeue)->attr.copy( data ); 
    new_node->next = NULL;
    new_node->prev = NULL;

    /* insert */
    if( (*dequeue)->count == 0 )
    {
        (*dequeue)->head = new_node;
        (*dequeue)->tail = new_node;
    }
    else
    {
        (*dequeue)->tail->next = new_node;
        new_node->prev = (*dequeue)->tail;
        (*dequeue)->tail = new_node;
    }
    (*dequeue)->count++;
    (*dequeue)->memory += sizeof( struct _dequeue_node );

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
    struct _dequeue_node * temp = NULL;

    /* check */
    if( !dequeue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_remove_front" );

        return -1;
    }

    if( !dequeue )
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
    temp = (*dequeue)->head;
    if( (*dequeue)->count == 1 )
    {
        (*dequeue)->head = NULL;
        (*dequeue)->tail = NULL;
    }
    else
    {
        (*dequeue)->head = temp->next;
        (*dequeue)->head->prev = NULL;
    }
    (*dequeue)->count--;
    (*dequeue)->memory -= sizeof( struct _dequeue_node );

    /* deallocate */
    (*dequeue)->attr.free( temp->data );
    temp->data = NULL;

    free( temp );
    temp = NULL;

    assert( (*dequeue)->count >= 0 );
    assert( (*dequeue)->memory >= sizeof( dequeue_t ) );

    return 0;
}

/**
 * Remove data from the back of the dequeue.
 */
int dequeue_remove_back( dequeue_t ** dequeue )
{
    struct _dequeue_node * temp = NULL;

    /* check */
    if( !dequeue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_remove_back" );

        return -1;
    }

    if( !dequeue )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "dequeue_remove_back" );

        return -1;
    }

    if( (*dequeue)->count == 0 )
    {
        error_code = ERROR_UNDERFLOW;
        error_print( "dequeue_remove_back" );

        return -1;
    }

    assert( (*dequeue)->attr.free );

    /* remove */
    temp = (*dequeue)->tail;
    if( (*dequeue)->count == 1 )
    {
        (*dequeue)->head = NULL;
        (*dequeue)->tail = NULL;
    }
    else
    {
        (*dequeue)->tail = temp->prev;
        (*dequeue)->tail->next = NULL;
    }
    (*dequeue)->count--;
    (*dequeue)->memory -= sizeof( struct _dequeue_node );

    /* deallocate */
    (*dequeue)->attr.free( temp->data );
    temp->data = NULL;

    free( temp );
    temp = NULL;

    assert( (*dequeue)->count >= 0 );
    assert( (*dequeue)->memory >= sizeof( dequeue_t ) );
   
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
    struct _dequeue_node * current = NULL;
    struct _dequeue_node * prev = NULL;
    struct _dequeue_node * temp = NULL;

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

    assert( (*dequeue)->attr.free );

    /* look for data */
    for( current = (*dequeue)->head, prev = NULL; current != NULL; )
    {
        if( (*dequeue)->attr.compare( current->data, data ) == 0 )
        {
            temp = current;
            current = current->next;

            /* remove from list */
            if( temp == (*dequeue)->head )
            {
                if( (*dequeue)->count == 1 )
                {
                    (*dequeue)->head = NULL;
                    (*dequeue)->tail = NULL;
                }
                else
                {
                    (*dequeue)->head = temp->next;
                    (*dequeue)->head->prev = NULL;
                }
            }
            else if( temp == (*dequeue)->tail )
            {
                if( (*dequeue)->count == 1 )
                {
                    (*dequeue)->head = NULL;
                    (*dequeue)->tail = NULL;
                }
                else
                {
                    (*dequeue)->tail = temp->prev;
                    (*dequeue)->tail->next = NULL;
                }
            }
            else
            {
                prev->next = temp->next;
                temp->prev = prev;
            }
            (*dequeue)->count--;
            (*dequeue)->memory -= sizeof( struct _dequeue_node );

            /* deallocate */
            (*dequeue)->attr.free( temp->data );
            temp->data = NULL;

            free( temp );
            temp = NULL;

            assert( (*dequeue)->count >= 0 );
            assert( (*dequeue)->memory >= sizeof( dequeue_t ) );

            return 0;
        }
        else
        {
            prev = current;
            current = current->next;
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
    struct _dequeue_node * current = NULL;
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
    nchr += fprintf( fp, " - count = %d\n", dequeue->count );
    nchr += fprintf( fp, " - memory = %d\n", dequeue->memory );

    for( current = dequeue->head; current != NULL; current = current->next )
    {
        nchr += dequeue->attr.print( current->data, fp );
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

    return dequeue->count;
}

/**
 * Get the capacity of the dequeue.
 *
 * This function is not implemented.
 */
inline int dequeue_capacity( const dequeue_t * dequeue )
{
    error_code = ERROR_NOT_IMPLEMENTED;
    error_print( "dequeue_capacity" );

    return -1;
}
