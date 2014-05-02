/**
 * This file provides a stack ADT.
 *
 * \brief This file provides a generic stack ADT
 * \author Julian Martinez del Campo
 * \license GNU Lesser General Public License (LGPL) https://www.gnu.org/copyleft/lesser.html
 *
 * A stack is initialized by first setting the stack attributes. The stack
 * attributes determine how the data is handled by stack. i.e. To properly
 * handle different data types such as integers or strings, the stack
 * attributes, or function pointers, must be set. 
 */
#ifndef _STACK_H_
#define _STACK_H_

#include <stdio.h>

#include <attributes.h>

typedef struct _stack_type stack_t;

/**
 * Create a new stack.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \param attr the data attribute. If the attribute is NULL then a stack is
 * created with the default settings.
 * \param capacity the capacity of the stack (must be greater than 0).
 * \return a new stack. If an error occurs then return NULL.
 */
stack_t * stack_create( const attr_t * attr, int capacity );

/**
 * Destroy a stack.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \post The stack and all data contained within is deallocated.
 * \param stack a stack to destroy.
 * \return 0 on success, return -1 on error.
 */
int stack_destroy( stack_t ** stack );

/**
 * Check if stack contains data.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \param stack a stack search through.
 * \param data the data to check the stack for.
 * \return 1 if data is in the the stack, return 0 if data is not in the stack,
 * return -1 on error.
 */
int stack_contains( const stack_t * stack, const void * data );

/**
 * Get data at the top of the stack.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \param stack a stack to retrieve data from.
 * \return the data at the top of the stack, return NULL if stack is empty,
 * return NULL on error. 
 */
void * stack_peek( const stack_t * stack );

/**
 * Insert data into the stack.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \post The data is inserted at the top of the stack.
 * \param stack a stack to insert data into.
 * \param data the data to insert into the stack.
 * \return 0 on success, return -1 on error.
 */
int stack_insert( stack_t ** stack, void * data );

/**
 * Remove data from the stack.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \post The data at the top of the stack is deallocated, and removed from
 * the stack.
 * \param stack the stack to remove data from.
 * \return 0 on success, return -1 on error.
 */
int stack_remove( stack_t ** stack );

/**
 * Push data into the stack.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \post The data is inserted at the top of the stack.
 * \param stack a stack to insert data into.
 * \param data the data to insert into the stack.
 * \return 0 on success, return -1 on error.
 */
int stack_push( stack_t ** stack, void * data );

/**
 * Pop data off the stack.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \post The data at the top of the stack is deallocated, and removed from
 * the stack.
 * \param stack the stack to remove data from.
 * \return 0 on success, return -1 on error.
 */
int stack_pop( stack_t ** stack );

/**
 * Get the number of items in the stack.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \param stack the stack to get the number of items from.
 * \return the number of items in the stack, return -1 on error.
 */
int stack_count( const stack_t * stack );

/**
 * Get the memory usage of the stack.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \param stack the stack to get memory usage of.
 * \return the size, in bytes, used by the stack, return -1 on error.
 */
int stack_memory( const stack_t * stack );

/**
 * Get the capacity of the stack.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \param stack the stack to get the capacity of.
 * \return the capacity of the stack, return -1 on error.
 */
int stack_capacity( const stack_t * stack );

/**
 * Print a stack.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \param stack the stack to print.
 * \param fp the file pointer to print to.
 * \return the number of characters printed, return -1 on error.
 */
int stack_print( const stack_t * stack, FILE * fp );

/**
 * Clear all data from the stack.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \post All data is deallocated and removed from the stack.
 * \param stack the stack to clear.
 * \return 0 on success, return -1 on error.
 */
int stack_clear( stack_t ** stack );

/**
 * Resize the stack.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \param stack the stack to resize.
 * \param capacity the new capacity of the stack. The new capacity must be
 * greater than the number of items currently in the stack.
 * \return 0 on success, return -1 on error.
 */
int stack_reserve( stack_t ** stack, int capacity );

/**
 * Remove item from the stack.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \post The data is removed from the stack. 
 * \param stack the stack to remove data from.
 * \return 0 on success, return -1 on error.
 */
int stack_remove_item( stack_t ** stack, void * data );

#endif /* _STACK_H_ */
