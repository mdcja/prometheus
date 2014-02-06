/**
 * This file provides an implementation of a stack using a linked list data
 * structure.
 *
 * @brief Provide a stack implemented using a linked list.
 * @author Julian Martinez del Campo
 * @license GNU Lesser General Public License (LGPL) https://www.gnu.org/copyleft/lesser.html
 * 
 * This implementation provides O(n) space complexity for a stack.
 *
 * This implementation provides the following run time complexity for a stack
 * with n elements:
 *
 * Operation    | Complexity
 * -------------|--------------
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
 *
 * @warning stack_capacity() and stack_reserve() are unimplemented
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "stack.h"
#include "error.h"
#include "attributes.h"

/**
 * A linked list node.
 */
struct _stack_node {
    void * data;
    struct _stack_node * next;
};

/**
 * A stack implemented with a linked list.
 */
struct _stack_type {
    struct _stack_node * head;
    int count;
    int memory;
    attr_t attr;
};

/**
 * Create a new stack.
 *
 * @note The capacity is ignored.
 *
 * Creates a new stack implemented using a linked list.
 */
stack_t * stack_create( const attr_t * attr, int capacity )
{
    stack_t * new_stack = NULL;

    /* create */
    new_stack = (stack_t *)calloc( 1, sizeof( stack_t ) );
    assert( new_stack );
    if( !new_stack )
    {
        error_code = ERROR_MEMORY_ALLOCATION_FAILED;
        error_print( "stack_create" );

        return NULL;
    }

    /* initialize */
    new_stack->head = NULL;
    new_stack->count = 0;
    new_stack->memory = sizeof( stack_t );
    if( attr )
    {
        new_stack->attr = *attr;
    }
    else
    {
        attr_init( &new_stack->attr );
    }

    assert( new_stack->attr.copy );
    assert( new_stack->attr.free );
    assert( new_stack->attr.compare );
    assert( new_stack->attr.print );

    return new_stack;
}

/**
 * Destroy a stack.
 *
 * First all items in the stack are deallocated from the stack, then the stack
 * is deallocated.
 */
int stack_destroy( stack_t ** stack )
{
    /* check */
    if( !stack )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "stack_destroy" );

        return -1;
    }

    if( !*stack )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "stack_destroy" );

        return -1;
    }

    /* clear stack */
    if( stack_clear( stack ) != 0 )
    {
        return -1;
    }

    /* destroy stack */
    free( *stack );
    *stack = NULL;

    return 0;
}

/**
 * Check if stack contains data.
 *
 * Check each node in the stack for the given data.
 */
int stack_contains( const stack_t * stack, const void * data )
{
    struct _stack_node * current = NULL;

    /* check */
    if( !stack )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "stack_contains" );

        return -1;
    }

    if( !data )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "stack_contains" );

        return -1;
    }

    assert( stack->attr.compare );

    /* find item */
    for( current = stack->head; current != NULL; current = current->next )
    {
        if( stack->attr.compare( current->data, data ) == 0 )
        {
            return 1;
        }
    }

    return 0;
}

/**
 * Get data at the top of the stack.
 */
inline void * stack_peek( const stack_t * stack )
{
    /* check */
    if( !stack )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "stack_peek" );

        return NULL;
    }

    /* get item */
    return stack->head ? stack->head->data : NULL;
}

/**
 * Insert data into the stack.
 *
 * Insert an item at the top of the stack.
 */
int stack_insert( stack_t ** stack, void * data )
{
    struct _stack_node * new_node = NULL;

    /* check */
    if( !stack )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "stack_insert" );

        return -1;
    }

    if( !*stack )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "stack_insert" );

        return -1;
    }

    if( !data )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "stack_insert" );

        return -1;
    }

    assert( (*stack)->attr.copy );

    /* create node */
    new_node = (struct _stack_node *)calloc( 1, sizeof( struct _stack_node ) );
    assert( new_node );
    if( !new_node )
    {
        error_code = ERROR_MEMORY_ALLOCATION_FAILED;
        error_print( "stack_insert" );

        return -1;
    }

    /* initialize */
    new_node->data = (*stack)->attr.copy( data );
    new_node->next = NULL;

    /* insert */
    new_node->next = (*stack)->head;
    (*stack)->head = new_node;

    (*stack)->count++;
    (*stack)->memory += sizeof( struct _stack_node );

    return 0;
}

/**
 * Remove data from the stack.
 */
int stack_remove( stack_t ** stack )
{
    struct _stack_node * temp = NULL;

    /* check */
    if( !stack )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "stack_remove" );

        return -1;
    }

    if( !*stack )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "stack_remove" );

        return -1;
    }

    if( (*stack)->count <= 0 )
    {
        error_code = ERROR_UNDERFLOW;
        error_print( "stack_remove" );

        return -1;
    }

    assert( (*stack)->attr.free );

    /* remove node */
    temp = (*stack)->head;
    (*stack)->head = temp->next;

    (*stack)->count--;
    (*stack)->memory -= sizeof( struct _stack_node );

    /* deallocate node */
    (*stack)->attr.free( temp->data );
    temp->data = NULL;

    free( temp );
    temp = NULL;

    assert( (*stack)->count >= 0 );
    assert( (*stack)->memory >= sizeof( stack_t ) );

    return 0;
}

/**
 * Push data into the stack.
 *
 * An alias for stack_insert().
 */
inline int stack_push( stack_t ** stack, void * data )
{
    return stack_insert( stack, data );
}

/**
 * Pop data off the stack.
 *
 * An alias for stack_remove().
 */
inline int stack_pop( stack_t ** stack )
{
    return stack_remove( stack );
}

/**
 * Get the number of items in the stack.
 */
inline int stack_count( const stack_t * stack )
{
    /* check */
    if( !stack )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "stack_count" );

        return -1;
    }

    assert( stack->count >= 0 );

    /* get count */
    return stack->count;
}

/**
 * Get the memory usage of the stack.
 */
inline int stack_memory( const stack_t * stack )
{
    /* check */
    if( !stack )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "stack_memory" );

        return -1;
    }

    assert( stack->memory >= sizeof( stack_t ) );

    /* get memory */
    return stack->memory;
}

/**
 * Get the capacity of the stack.
 *
 * This function is unimplemented as it does not make sense for a linked list
 * data structure.
 */
inline int stack_capacity( const stack_t * stack )
{
    /* check */
    error_code = ERROR_NOT_IMPLEMENTED;
    error_print( "stack_capacity" );

    return -1;
}

/**
 * Print a stack.
 */
int stack_print( const stack_t * stack, FILE * fp )
{
    struct _stack_node * current = NULL;
    int nchr = 0;

    /* check */
    if( !stack )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "stack_print" );

        return -1;
    }

    if( !fp )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "stack_print" );

        return -1;
    }

    assert( stack->attr.print );

    /* print */
    nchr += fprintf( fp, "Stack (%p)\n", (void *)stack );
    nchr += fprintf( fp, " - count = %d\n", stack->count );
    nchr += fprintf( fp, " - memory = %d\n", stack->memory );

    for( current = stack->head; current != NULL; current = current->next )
    {
        nchr += stack->attr.print( current->data, fp );
        nchr += fprintf( fp, "\n" );
    }

    return nchr;
}

/**
 * Clear all data from the stack.
 */
int stack_clear( stack_t ** stack )
{
    struct _stack_node * current = NULL;
    struct _stack_node * temp = NULL;

    /* check */
    if( !stack )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "stack_clear" );

        return -1;
    }

    if( !*stack )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "stack_clear" );

        return -1;
    }

    assert( (*stack)->attr.free );

    /* clear each node */
    for( current = (*stack)->head; current != NULL; )
    {
        temp = current;
        current = current->next;

        /* deallocate node */    
        (*stack)->attr.free( temp->data );
        temp->data = NULL;

        free( temp );
        temp = NULL;

        (*stack)->count--;
        (*stack)->memory -= sizeof( struct _stack_node );
    }
    (*stack)->head = NULL;

    assert( (*stack)->count == 0 );
    assert( (*stack)->memory == sizeof( stack_t ) );

    return 0;
}

/**
 * Resize the stack.
 * 
 * This function is unimplemented as it does not make sense for a linked list
 * data structure.
 */
inline int stack_reserve( stack_t ** stack, int capacity )
{
    /* check */
    error_code = ERROR_NOT_IMPLEMENTED;
    error_print( "stack_reserve" );

    return -1;
}

/**
 * Remove item from the stack.
 */
int stack_remove_item( stack_t ** stack, void * data )
{
    struct _stack_node * current = NULL;
    struct _stack_node * prev = NULL;
    struct _stack_node * temp = NULL;

    /* check */
    if( !stack )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "stack_remove_item" );

        return -1;
    }

    if( !*stack )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "stack_remove_item" );

        return -1;
    }

    if( !data )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "stack_remove_item" );

        return -1;
    }

    assert( (*stack)->attr.compare );
    assert( (*stack)->attr.free );

    /* find item */
    for( current = (*stack)->head, prev = NULL; current != NULL; )
    {
        if( (*stack)->attr.compare( current->data, data ) == 0 )
        {
            /* remove item */
            temp = current;

            if( current == (*stack)->head )
            {
                (*stack)->head = current->next;
            }
            else
            {
                prev->next = current->next;
            }
            current = current->next;

            /* deallocate node */
            (*stack)->attr.free( temp->data );
            temp->data = NULL;

            free( temp );
            temp = NULL;

            (*stack)->count--;
            (*stack)->memory -= sizeof( struct _stack_node );

            return 0;
        }
        else
        {
            prev = current;
            current = current->next;
        }
    }

    error_code = ERROR_NOT_FOUND;
    error_print( "stack_remove_item" );

    return -1;
}
