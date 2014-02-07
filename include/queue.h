/**
 * This file provides a generic FIFO queue ADT.
 *
 * @brief This file provides a FIFO queue ADT
 * @author Julian Martinez del Campo
 * @license GNU Lesser General Public License (LGPL) https://www.gnu.org/copyleft/lesser.html
 *
 * The attributes determine how the data is handled by queue. i.e. To properly
 * handle different data types such as integers or strings, the queue
 * attributes, or function pointers, must be set. 
 */
#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdio.h>

#include "attributes.h"

typedef struct _queue_type queue_t;

/**
 * Create a new queue.
 *
 * @post If an error occurs, then error_code is set (see error.h).
 * @param attr the data attribute. If the attribute is NULL then a queue is
 * created with the default settings.
 * @param capacity the capacity of the queue. Ignored in this implementation.
 * @return a new queue. If an error occurs then return NULL.
 */
queue_t * queue_create( const attr_t * attr, int capacity );

/**
 * Destroy a queue.
 *
 * @post If an error occurs, then error_code is set (see error.h).
 * @post The queue and all data contained within is deallocated.
 * @param queue a queue to destroy.
 * @return 0 on success, return -1 on error.
 */
int queue_destroy( queue_t ** queue );

/**
 * Resize the queue.
 *
 * @post If an error occurs, then error_code is set (see error.h).
 * @param queue the queue to resize.
 * @param capacity the new capacity of the queue.
 * @return return 0 on success, return -1 on error.
 */
int queue_reserve( queue_t ** queue, int capacity );

/**
 * Clear all data from the queue.
 *
 * @post If an error occurs, then error_code is set (see error.h).
 * @post All data is deallocated and removed from the queue.
 * @param queue the queue to clear.
 * @return 0 on success, return -1 on error.
 */
int queue_clear( queue_t ** queue );

/**
 * Get data at the front of the queue.
 *
 * @post If an error occurs, then error_code is set (see error.h).
 * @param queue a queue to retrieve data from.
 * @return the data at the front of the queue, return NULL if queue is empty,
 * return NULL on error. 
 */
void * queue_peek( const queue_t * queue );

/**
 * Check if queue contains data.
 *
 * @post If an error occurs, then error_code is set (see error.h).
 * @param queue a queue to destroy.
 * @param data the data to check the queue for.
 * @return 1 if data is in the the queue, return 0 if data is not in the queue,
 * return -1 on error.
 */
int queue_contains( const queue_t * queue, const void * data );

/**
 * Insert data into the queue.
 *
 * @post If an error occurs, then error_code is set (see error.h).
 * @post The data is inserted at the back of the queue.
 * @param queue a queue to insert data into.
 * @param data the data to insert into the queue.
 * @return 0 on success, return -1 on error.
 */
int queue_insert( queue_t ** queue, void * data );

/**
 * Push data into the queue.
 *
 * This function is an alias for queue_insert.
 *
 * @post If an error occurs, then error_code is set (see error.h).
 * @post The data is inserted at the back of the queue.
 * @param queue a queue to insert data into.
 * @param data the data to insert into the queue.
 * @return 0 on success, return -1 on error.
 */
int queue_push( queue_t ** queue, void * data );

/**
 * Remove data from the queue.
 *
 * @post If an error occurs, then error_code is set (see error.h).
 * @post The data at the front of the queue is deallocated, and removed from
 * the queue.
 * @param queue the queue to remove data from.
 * @return 0 on success, return -1 on error.
 */
int queue_remove( queue_t ** queue );

/**
 * Pop data from the queue.
 *
 * This function is an alias for queue_remove.
 *
 * @post If an error occurs, then error_code is set (see error.h).
 * @post The data at the front of the queue is deallocated, and removed from
 * the queue.
 * @param queue the queue to remove data from.
 * @return 0 on success, return -1 on error.
 */
int queue_pop( queue_t ** queue );

/**
 * Remove data from the queue.
 *
 * @post If an error occurs, then error_code is set (see error.h).
 * @post The data is removed from the queue. 
 * @param queue the queue to remove data from.
 * @return 0 on success, return -1 on error.
 */
int queue_remove_item( queue_t ** queue, void * data );

/**
 * Print a queue.
 *
 * @post If an error occurs, then error_code is set (see error.h).
 * @param queue the queue to print.
 * @param fp the file pointer to print to.
 * @return the number of characters printed, return -1 on error.
 */
int queue_print( const queue_t * queue, FILE * fp );

/**
 * Get the memory usage of the queue.
 *
 * @post If an error occurs, then error_code is set (see error.h).
 * @param queue the queue to get memory usage of.
 * @return the size, in bytes, used by the queue, return -1 on error.
 */
int queue_memory( const queue_t * queue );

/**
 * Get the number of items in the queue.
 *
 * @post If an error occurs, then error_code is set (see error.h).
 * @param queue the queue to get the number of items from.
 * @return the number of items in the queue, return -1 on error.
 */
int queue_count( const queue_t * queue );

/**
 * Get the capacity of the queue.
 *
 * @post If an error occurs, then error_code is set (see error.h).
 * @param queue the queue to get the capacity of.
 * @return return the capacity of the queue, return -1 on error.
 */
int queue_capacity( const queue_t * queue );

#endif /* _QUEUE_H_ */
