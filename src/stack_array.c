/**
 * This file provides an implementation of a stack using an array data
 * structure.
 *
 * \brief Provide a stack implemented using an array.
 * \author Julian Martinez del Campo
 * \license GNU Lesser General Public License (LGPL) https://www.gnu.org/copyleft/lesser.html
 *
 * This implementation provides O(1) space complexity for a stack.
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
 * capacity     | O(1)
 * print        | O(n)
 * clear        | O(n)
 * reserve      | O(n)
 * remove item  | O(n)
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "attributes.h"
#include "stack.h"
#include "error.h"

/**
 * The stack data structure.
 */
struct _stack_type {
    void ** stack;
    int capacity;
    int count;
    int memory;
    attr_t attr;
};

/**
 * Create a new stack.
 *
 * The stack data structure is allocated and initialized.
 */
stack_t * stack_create( const attr_t * attr, int capacity )
{
    stack_t * new_stack = NULL;

    /* check */
    if( capacity < 1 )
    {
        error_code = ERROR_INVALID_CAPACITY;
        error_print( "stack_create" );

        return NULL;
    }

    /* create */
    new_stack = (stack_t *)calloc( 1, sizeof( stack_t ) );
    assert( new_stack );
    if( !new_stack )
    {
        error_code = ERROR_MEMORY_ALLOCATION_FAILED;
        error_print( "stack_create" );

        return NULL;
    }

    new_stack->stack = (void **)calloc( capacity, sizeof( void * ) );
    assert( new_stack->stack );
    if( !new_stack->stack )
    {
        error_code = ERROR_MEMORY_ALLOCATION_FAILED;
        error_print( "stack_create" );

        return NULL;
    }

    /* initialize */
    new_stack->capacity = capacity;
    new_stack->count = 0;
    new_stack->memory = sizeof( stack_t ) + capacity * sizeof( void * );
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
 * All data in the stack is first deallocated using the data_free() function
 * set in the attributes. The data structure is then deallocated from memory
 * and set to NULL.
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

    /* clear */
    if( stack_clear( stack ) != 0 )
    {
        return -1;
    }

    /* destroy */
    free( (*stack)->stack );
    (*stack)->stack = NULL;

    free( *stack );
    *stack = NULL;

    return 0;
}

/**
 * Check if stack contains data.
 *
 * Each element in the stack is checked for the data using the data_compare()
 * function set in the attributes.
 */
int stack_contains( const stack_t * stack, const void * data )
{
    int i = 0;

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

    /* search for item */
    for( i = 0; i < stack->count; ++i )
    {
        if( stack->attr.compare( stack->stack[ i ], data ) == 0 )
        {
            return 1;
        }
    }

    return 0;
}

/**
 * Get data at the top of the stack.
 *
 * Retrieve the data at the top of the stack, if the count in the stack is zero
 * then return NULL, otherwise return the data at the top.
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

    return stack->count ? stack->stack[ stack->count - 1 ] : NULL;
}

/**
 * Insert data into the stack.
 *
 * Insert the data at the end of the array, if capacity is not exceeded.
 */
int stack_insert( stack_t ** stack, void * data )
{
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
    
    if( (*stack)->count >= (*stack)->capacity )
    {
        error_code = ERROR_OVERFLOW;
        error_print( "stack_insert" );

        return -1;
    }

    assert( (*stack)->attr.copy );

    /* insert */
    (*stack)->stack[ (*stack)->count ] = (*stack)->attr.copy( data );
    (*stack)->count++;

    assert( (*stack)->count <= (*stack)->capacity );

    return 0;
}

/**
 * Remove data from the stack.
 *
 * Removes data from the top of the stack if there are items in the stack.
 */
int stack_remove( stack_t ** stack )
{
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

    /* remove */
    (*stack)->attr.free( (*stack)->stack[ (*stack)->count - 1 ] );
    (*stack)->stack[ (*stack)->count - 1 ] = NULL;
    (*stack)->count--;

    assert( (*stack)->count >= 0 );

    return 0;
}

/**
 * Push data into the stack.
 */
inline int stack_push( stack_t ** stack, void * data )
{
    return stack_insert( stack, data );
}

/**
 * Pop data off the stack.
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

    assert( stack->memory > 0 );
    
    return stack->memory;
}

/**
 * Get the capacity of the stack.
 */
inline int stack_capacity( const stack_t * stack )
{
    /* check */
    if( !stack )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "stack_capacity" );

        return -1;
    }

    assert( stack->capacity > 0 );

    return stack->capacity;
}

/**
 * Print a stack.
 *
 * Prints information about the stack, such as the stack location, its
 * capacity, count, memory usage, and all data contained within.
 */
int stack_print( const stack_t * stack, FILE * fp )
{
    int i = 0;
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
    nchr += fprintf( fp, " - capacity = %d\n", stack->capacity );
    nchr += fprintf( fp, " - count = %d\n", stack->count );
    nchr += fprintf( fp, " - memory = %d\n", stack->memory );

    /* print starting with the top of the stack */
    for( i = stack->count; i > 0; --i )
    {
        nchr += stack->attr.print( stack->stack[ i - 1 ], fp );
        nchr += fprintf( fp, "\n" );
    }

    return nchr;
}

/**
 * Clear all data from the stack.
 *
 * Deallocate all the data contained in the stack.
 */
int stack_clear( stack_t ** stack )
{
    int i = 0;

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

    /* clear */
    for( i = 0; (*stack)->count > 0; ++i )
    {
        (*stack)->attr.free( (*stack)->stack[ i ] );
        (*stack)->stack[ i ] = NULL;
        (*stack)->count--;
    }

    assert( (*stack)->count == 0 );

    return 0;
}

/**
 * Resize the stack.
 *
 * Create a new array that is of the desired capacity and copy over all values,
 * then deallocate the old array.
 */
int stack_reserve( stack_t ** stack, int capacity )
{
    void ** new_stack = NULL;
    void ** old_stack = NULL;
    int i = 0;

    /* check */
    if( !stack )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "stack_reserve" );

        return -1;
    }

    if( !*stack )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "stack_reserve" );

        return -1;
    }

    if( capacity <= (*stack)->count )
    {
        error_code = ERROR_INVALID_CAPACITY;
        error_print( "stack_reserve" );
    }

    /* create */
    new_stack = (void **)calloc( capacity, sizeof( void * ) );
    assert( new_stack );
    if( !new_stack )
    {
        error_code = ERROR_MEMORY_ALLOCATION_FAILED;
        error_print( "stack_reserve" );

        return -1;
    }

    /* copy */
    for( i = 0; i < (*stack)->count; ++i )
    {
        new_stack[ i ] = (*stack)->stack[ i ];
    }

    /* switch */
    old_stack = (*stack)->stack;
    (*stack)->stack = new_stack;
    (*stack)->capacity = capacity;
    (*stack)->memory = sizeof( stack_t ) + capacity * sizeof( void * );

    /* destroy */
    free( old_stack );
    old_stack = NULL;
    
    return 0;
}

/**
 * Remove item from the stack.
 *
 * Check each element in the stack and look for data. If found deallocate the
 * data, and shift the array over.
 */
int stack_remove_item( stack_t ** stack, void * data )
{
    int i = 0;

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

    /* find data */
    for( i = 0; i < (*stack)->count; ++i )
    {
        if( (*stack)->attr.compare( (*stack)->stack[ i ], data ) == 0 )
        {
            (*stack)->attr.free( (*stack)->stack[ i ] );
            (*stack)->stack[ i ] = NULL;
            (*stack)->count--;

            /* shift */
            for( ; i < (*stack)->count; ++i )
            {
                (*stack)->stack[ i ] = (*stack)->stack[ i + 1 ];
            }
            (*stack)->stack[ i ] = NULL;

            assert( (*stack)->count >= 0 );

            return 0;
        }
    }

    error_code = ERROR_NOT_FOUND;
    error_print( "stack_remove_item" );

    return -1;
}
