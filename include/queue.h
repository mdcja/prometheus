/* Author: Julian Martinez del Campo
 *
 * This file provides a queue ADT. The goal of this file is to provide an easy
 * to use, but flexible FIFO queue.
 *
 * If _EMBEDDED macro is defined then all functions depending on stdio.h are
 * disabled. This may be useful when programming on embedded systems.
 */
#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "attributes.h"

/* Define a generic queue type 
 */ 
typedef struct _queue_type queue_t;

/* Create a queue. If attr is NULL then use default attributes. This function
 * returns a new queue on success, else returns NULL on error.
 */
queue_t * queue_create( const attr_t * attr, int capacity );

/* Destroy a queue. This function will deallocate all items in the queue
 * (according to attributes), and deallocate the queue from memory. This
 * function returns 0 on success, else returns -1 on error.
 */
int queue_destroy( queue_t ** queue );

/* Resize a queue. This function will resize the queue to a new valid capacity.
 * This function returns 0 on success, else returns -1 on error.
 */
int queue_reserve( queue_t ** queue, int capacity );

/* Remove all items from the queue. Returns 0 on success, else returns -1 on
 * error.
 */
int queue_clear( queue_t ** queue );

/* Get the first item in the queue. Returns the first item in the queue, else
 * returns NULL if the queue is empty or if an error ocurred.
 */
void * queue_peek( const queue_t * queue );

/* Check if a queue contains a specific item. If the item is found then return
 * 1, if the item was not found return 0, else if an error occured return -1.
 */
int queue_contains( const queue_t * queue, const void * data );

/* Insert an item into the end of the queue. Returns 0 on success, else returns
 * -1 on error.
 */
int queue_insert( queue_t ** queue, void * data );

/* Remove an item from the front of the queue. Returns 0 on success, else
 * returns -1 on error.
 */
int queue_remove( queue_t ** queue );

/* Remove a specified item from the queue. This will remove an item from the
 * queue regardless of its position in the queue if it is found. Returns 0 on
 * success, else returns -1 on error.
 */
int queue_remove_item( queue_t ** queue, void * data );

#ifndef _EMBEDDED
/* Print information about the queue as well as all the items contained in the
 * queue. Returns the number of characters printed, else returns -1 on error.
 */
int queue_print( const queue_t * queue, FILE * fp );
#endif /* _EMBEDDED */

/* Get the amount of memory being used by the queue. Returns the number of
 * bytes being used by the queue, else returns -1 on error.
 */
int queue_memory( const queue_t * queue );

/* Get the number of items in the queue. Returns the number of items on the
 * queue, else returns -1 on error.
 */
int queue_count( const queue_t * queue );

/* Get the capacity of the queue. Returns the capacity of the queue, else
 * returns -1 on error.
 */
int queue_capacity( const queue_t * queue );

#endif /* _QUEUE_H_ */
