/**
 * This file provides a generic double ended queue (dequeue) ADT.
 *
 * \brief This file provides a dequeue ADT
 * \author Julian Martinez del Campo
 * \license GNU Lesser General Public License (LGPL) https://www.gnu.org/copyleft/lesser.html
 *
 * A dequeue is a combination of a queue and a stack, allowing for removal and
 * insertion at both the front and rear of the queue.
 */
#ifndef _DEQUEUE_H_
#define _DEQUEUE_H_

#include <stdio.h>

#include "attributes.h"

typedef struct _dequeue_type dequeue_t;

/**
 * Create a new dequeue.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \param attr the data attribute. If the attribute is NULL then a dequeue is
 * created with the default settings.
 * \param capacity the capacity of the dequeue.
 * \return a new dequeue. If an error occurs then return NULL.
 */
dequeue_t * dequeue_create( const attr_t * attr, int capacity );

/**
 * Destroy a dequeue.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \post The dequeue and all data contained within is deallocated.
 * \param dequeue a dequeue to destroy.
 * \return 0 on success, return -1 on error.
 */
int dequeue_destroy( dequeue_t ** dequeue );

/**
 * Resize the dequeue.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \param dequeue the dequeue to resize.
 * \param capacity the new capacity of the dequeue.
 * \return 0 on success, return -1 on error.
 */
int dequeue_reserve( dequeue_t ** dequeue, int capacity );

/**
 * Clear all data from the dequeue.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \post All data is deallocated and removed from the dequeue.
 * \param dequeue the dequeue to clear.
 * \return 0 on success, return -1 on error.
 */
int dequeue_clear( dequeue_t ** dequeue );

/**
 * Get data at the front of the dequeue.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \param dequeue a dequeue to retrieve data from.
 * \return the data at the front of the dequeue, return NULL if dequeue is empty,
 * return NULL on error. 
 */
void * dequeue_peek_front( const dequeue_t * dequeue );

/**
 * Get data at the back of the dequeue.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \param dequeue a dequeue to retrieve data from.
 * \return the data at the back of the dequeue, return NULL if dequeue is empty,
 * return NULL on error. 
 */
void * dequeue_peek_back( const dequeue_t * dequeue );

/**
 * Check if dequeue contains data.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \param dequeue a dequeue to destroy.
 * \param data the data to check the dequeue for.
 * \return 1 if data is in the the dequeue, return 0 if data is not in the dequeue,
 * return -1 on error.
 */
int dequeue_contains( const dequeue_t * dequeue, const void * data );

/**
 * Insert data into the front of the dequeue.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \post The data is inserted at the front of the dequeue.
 * \param dequeue a dequeue to insert data into.
 * \param data the data to insert into the dequeue.
 * \return 0 on success, return -1 on error.
 */
int dequeue_insert_front( dequeue_t ** dequeue, void * data );

/**
 * Insert data into the dequeue.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \post The data is inserted at the back of the dequeue.
 * \param dequeue a dequeue to insert data into.
 * \param data the data to insert into the dequeue.
 * \return 0 on success, return -1 on error.
 */
int dequeue_insert_back( dequeue_t ** dequeue, void * data );

/**
 * Push data into the front of the dequeue.
 *
 * This function is an alias for dequeue_insert_front.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \post The data is inserted at the front of the dequeue.
 * \param dequeue a dequeue to insert data into.
 * \param data the data to insert into the dequeue.
 * \return 0 on success, return -1 on error.
 */
int dequeue_push_front( dequeue_t ** dequeue, void * data );

/**
 * Push data into the back of the dequeue.
 *
 * This function is an alias for dequeue_insert_back.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \post The data is inserted at the back of the dequeue.
 * \param dequeue a dequeue to insert data into.
 * \param data the data to insert into the dequeue.
 * \return 0 on success, return -1 on error.
 */
int dequeue_push_back( dequeue_t ** dequeue, void * data );

/**
 * Remove data from the front of the dequeue.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \post The data at the front of the dequeue is deallocated, and removed from
 * the dequeue.
 * \param dequeue the dequeue to remove data from.
 * \return 0 on success, return -1 on error.
 */
int dequeue_remove_front( dequeue_t ** dequeue );

/**
 * Remove data from the back of the dequeue.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \post The data at the back of the dequeue is deallocated, and removed from
 * the dequeue.
 * \param dequeue the dequeue to remove data from.
 * \return 0 on success, return -1 on error.
 */
int dequeue_remove_back( dequeue_t ** dequeue );

/**
 * Pop data from the front of the dequeue.
 *
 * This function is an alias for dequeue_remove_front.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \post The data at the front of the dequeue is deallocated, and removed from
 * the dequeue.
 * \param dequeue the dequeue to remove data from.
 * \return 0 on success, return -1 on error.
 */
int dequeue_pop_front( dequeue_t ** dequeue );

/**
 * Pop data from the back of the dequeue.
 *
 * This function is an alias for dequeue_remove_back.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \post The data at the back of the dequeue is deallocated, and removed from
 * the dequeue.
 * \param dequeue the dequeue to remove data from.
 * \return 0 on success, return -1 on error.
 */
int dequeue_pop_back( dequeue_t ** dequeue );

/**
 * Remove data from the dequeue.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \post The data is removed from the dequeue. 
 * \param dequeue the dequeue to remove data from.
 * \return 0 on success, return -1 on error.
 */
int dequeue_remove_item( dequeue_t ** dequeue, void * data );

/**
 * Print a dequeue.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \param dequeue the dequeue to print.
 * \param fp the file pointer to print to.
 * \return the number of characters printed, return -1 on error.
 */
int dequeue_print( const dequeue_t * dequeue, FILE * fp );

/**
 * Get the memory usage of the dequeue.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \param dequeue the dequeue to get memory usage of.
 * \return the size, in bytes, used by the dequeue, return -1 on error.
 */
int dequeue_memory( const dequeue_t * dequeue );

/**
 * Get the number of items in the dequeue.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \param dequeue the dequeue to get the number of items from.
 * \return the number of items in the dequeue, return -1 on error.
 */
int dequeue_count( const dequeue_t * dequeue );

/**
 * Get the capacity of the dequeue.
 *
 * \post If an error occurs, then error_code is set (see error.h).
 * \param dequeue the dequeue to get the capacity of.
 * \return 0 on success, return -1 on error.
 */
int dequeue_capacity( const dequeue_t * dequeue );

#endif /* _DEQUEUE_H_ */
