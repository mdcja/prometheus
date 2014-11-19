/* Author: Julian Martinez del Campo
 *
 * This file provides integration unit tests for the queue ADT. Unit tests are
 * run using the Check framework available at http://check.sourceforge.net/.
 */
#include <stdlib.h>
#include <stdio.h>
#include <check.h>

#include "queue.h"
#include "attributes.h"
#include "error.h"

#define DEFAULT_CAPACITY 4 
#define SECONDARY_CAPACITY 200

START_TEST( create_and_destroy )
{
    queue_t * queue = NULL;
    int rc = -1;
    
    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_peek( queue ) == NULL );
    ck_assert( queue_count( queue ) == 0 );
    ck_assert( queue_memory( queue ) > 0 );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
    ck_assert( queue == NULL );
}
END_TEST

START_TEST( error_create_negative_capacity )
{
    queue_t * queue = NULL;

    /* create with invalid capacity */
    queue = queue_create( NULL, -1 );
    ck_assert( queue == NULL );
    ck_assert( error_code == E_INVALID_CAPACITY );
}
END_TEST

START_TEST( error_create_zero_capacity )
{
    queue_t * queue = NULL;

    /* create with invalid capacity */
    queue = queue_create( NULL, 0 );
    ck_assert( queue == NULL );
    ck_assert( error_code == E_INVALID_CAPACITY );
}
END_TEST

START_TEST( error_destroy_null_queue )
{
    queue_t * queue = NULL;
    int rc = 0;
    
    /* destroy un-initialized queue */
    rc = queue_destroy( &queue );
    ck_assert( rc == -1 );
    ck_assert( error_code == E_NULL_PARAMETER );
}
END_TEST

START_TEST( reserve_empty_queue )
{
    queue_t * queue = NULL;
    int rc = -1;

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    rc = queue_reserve( &queue, SECONDARY_CAPACITY );
    ck_assert( rc == 0 );
    ck_assert( queue_count( queue ) == 0 );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( reserve_full_queue )
{
    queue_t * queue = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = {0};
    int i = 0;

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    /* fill queue */
    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_insert( &queue, &numbers[ i ] );
        ck_assert( rc == 0 );
        ck_assert( queue_count( queue ) == i + 1 );
    }
    ck_assert( queue_count( queue ) == DEFAULT_CAPACITY );

    rc = queue_reserve( &queue, SECONDARY_CAPACITY );
    ck_assert( rc == 0 );
    ck_assert( queue_count( queue ) == DEFAULT_CAPACITY );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( reserve_decrease_size )
{
    queue_t * queue = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = {0};
    int i = 0;

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    queue = queue_create( NULL, SECONDARY_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    /* add items to queue */
    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_insert( &queue, &numbers[ i ] );
        ck_assert( rc == 0 );
        ck_assert( queue_count( queue ) == i + 1 );
    }
    ck_assert( queue_count( queue ) == DEFAULT_CAPACITY );

    rc = queue_reserve( &queue, DEFAULT_CAPACITY );
    ck_assert( rc == 0 );
    ck_assert( queue_count( queue ) == DEFAULT_CAPACITY );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( error_reserve_invalid_capacity )
{
    queue_t * queue = NULL;
    int rc = -1;

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    rc = queue_reserve( &queue, -1 );
    ck_assert( rc == -1 );
    ck_assert( error_code == E_INVALID_CAPACITY );
    ck_assert( queue_count( queue ) == 0 );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( error_reserve_null_queue )
{
    queue_t * queue = NULL;
    int rc = 0;
    
    rc = queue_reserve( &queue, DEFAULT_CAPACITY );
    ck_assert( rc == -1 );
    ck_assert( error_code == E_NULL_PARAMETER );
}
END_TEST

START_TEST( clear_empty_queue )
{
    queue_t * queue = NULL;
    int rc = -1;

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    rc = queue_clear( &queue );
    ck_assert( rc == 0 );
    ck_assert( queue_count( queue ) == 0 );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( clear_full_queue )
{
    queue_t * queue = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = { 0 };
    int i = 0;

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    /* fill queue */
    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_insert( &queue, &numbers[ i ] );
        ck_assert( rc == 0 );
        ck_assert( queue_count( queue ) == i + 1 );
    }
    ck_assert( queue_count( queue ) == DEFAULT_CAPACITY );

    rc = queue_clear( &queue );
    ck_assert( rc == 0 );
    ck_assert( queue_count( queue ) == 0 );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( error_clear_null_queue )
{
    queue_t * queue = NULL;
    int rc = -1;

    rc = queue_clear( &queue );
    ck_assert( rc == -1 );
    ck_assert( error_code == E_NULL_PARAMETER );
}
END_TEST

START_TEST( peek_empty_queue )
{
    queue_t * queue = NULL;
    int * front = NULL;
    int rc = -1;

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    front = (int *)queue_peek( queue );
    ck_assert( front == NULL );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( peek_full_queue )
{
    queue_t * queue = NULL;
    int * front = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = { 0 };
    int i = 0;

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    /* fill queue */
    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_insert( &queue, &numbers[ i ] );
        ck_assert( rc == 0 );
        ck_assert( queue_count( queue ) == i + 1 );
    }
    ck_assert( queue_count( queue ) == DEFAULT_CAPACITY );

    front = (int *)queue_peek( queue );
    ck_assert( front == &numbers[ 0 ] );
    ck_assert( *front == numbers[ 0 ] );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( peek_remove_items_from_full_queue )
{
    queue_t * queue = NULL;
    int * front = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = { 0 };
    int i = 0;

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    /* fill queue */
    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_insert( &queue, &numbers[ i ] );
        ck_assert( rc == 0 );
        ck_assert( queue_count( queue ) == i + 1 );
    }
    ck_assert( queue_count( queue ) == DEFAULT_CAPACITY );

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        front = (int *)queue_peek( queue );
        ck_assert( front == &numbers[ i ] );
        ck_assert( *front == numbers[ i ] );

        rc = queue_remove( &queue );
        ck_assert( rc == 0 );
        ck_assert( queue_count( queue ) == DEFAULT_CAPACITY - ( i + 1 ) );
    }
    ck_assert( queue_count( queue ) == 0 );

    front = queue_peek( queue );
    ck_assert( front == NULL );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( error_peek_null_queue )
{
    queue_t * queue = NULL;
    int * front = NULL;

    front = (int *)queue_peek( queue );
    ck_assert( front == NULL );
    ck_assert( error_code == E_NULL_PARAMETER );
}
END_TEST

START_TEST( contains_empty_queue )
{
    queue_t * queue = NULL;
    int rc = -1;
    int object = 42;

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    rc = queue_contains( queue, &object );
    ck_assert( rc == 0 );
    
    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( contains_in_queue )
{
    queue_t * queue = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = { 0 };
    int i = 0;

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_insert( &queue, &numbers[ i ] );
        ck_assert( rc == 0 );
        ck_assert( queue_count( queue ) == i + 1 );
    }

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_contains( queue, &numbers[ i ] );
        ck_assert( rc == 1 );
    }

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( contains_not_in_queue )
{
    queue_t * queue = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = { 0 };
    int other_numbers[ DEFAULT_CAPACITY ] = { 0 };
    int i = 0;

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
        other_numbers[ i ] = DEFAULT_CAPACITY + i + 1;
    }

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_insert( &queue, &numbers[ i ] );
        ck_assert( rc == 0 );
        ck_assert( queue_count( queue ) == i + 1 );
    }

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_contains( queue, &other_numbers[ i ] );
        ck_assert( rc == 0 );
    }
    
    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( error_contains_null_queue )
{
    queue_t * queue = NULL;
    int number = 1;
    int rc = 0;

    rc = queue_contains( queue, &number );
    ck_assert( rc == -1 );
    ck_assert( error_code == E_NULL_PARAMETER );
}
END_TEST

START_TEST( error_contains_null_object )
{
    queue_t * queue = NULL;
    int * number = NULL;
    int rc = 0;

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    rc = queue_contains( queue, number );
    ck_assert( rc == -1 );
    ck_assert( error_code == E_NULL_PARAMETER );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( insert_empty_queue )
{
    queue_t * queue = NULL;
    int rc = -1;
    int object = 42;

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    rc = queue_insert( &queue, &object );
    ck_assert( rc == 0 );
    ck_assert( queue_count( queue ) == 1 );
    ck_assert( (int *)queue_peek( queue ) == &object );
    ck_assert( *(int *)queue_peek( queue ) == object );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( insert_objects )
{
    queue_t * queue = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = { 0 };
    int i = 0;

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_insert( &queue, &numbers[ i ] );
        ck_assert( rc == 0 );
        ck_assert( queue_count( queue ) == i + 1 );
        ck_assert( (int *)queue_peek( queue ) == &numbers[ 0 ] );
        ck_assert( *(int *)queue_peek( queue ) == numbers[ 0 ] );
    }
    ck_assert( queue_count( queue ) == DEFAULT_CAPACITY );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( error_insert_full_queue )
{
    queue_t * queue = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = { 0 };
    int i = 0;

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_insert( &queue, &numbers[ i ] );
        ck_assert( rc == 0 );
        ck_assert( queue_count( queue ) == i + 1 );
        ck_assert( (int *)queue_peek( queue ) == &numbers[ 0 ] );
        ck_assert( *(int *)queue_peek( queue ) == numbers[ 0 ] );
    }
    ck_assert( queue_count( queue ) == DEFAULT_CAPACITY );

    rc = queue_insert( &queue, &numbers[ 0 ] );
    ck_assert( rc == -1 );
    ck_assert( error_code == E_OVERFLOW );
    ck_assert( queue_count( queue ) == DEFAULT_CAPACITY );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( error_insert_null_queue )
{
    queue_t * queue = NULL;
    int rc = -1;
    int object = 42;

    rc = queue_insert( &queue, &object );
    ck_assert( rc == -1 );
    ck_assert( error_code == E_NULL_PARAMETER );
}
END_TEST

START_TEST( error_insert_null_object )
{
    queue_t * queue = NULL;
    int rc = -1;
    int * object = NULL;

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    rc = queue_insert( &queue, object );
    ck_assert( rc == -1 );
    ck_assert( error_code == E_NULL_PARAMETER );
    ck_assert( queue_count( queue ) == 0 );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( remove_one_object )
{
    queue_t * queue = NULL;
    int rc = -1;
    int object = 42;

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    rc = queue_insert( &queue, &object );
    ck_assert( rc == 0 );
    ck_assert( queue_count( queue ) == 1 );

    rc = queue_remove( &queue );
    ck_assert( rc == 0 );
    ck_assert( queue_count( queue ) == 0 );
    ck_assert( (int *)queue_peek( queue ) == NULL );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( remove_full_queue )
{
    queue_t * queue = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = { 0 };
    int i = 0;

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_insert( &queue, &numbers[ i ] );
        ck_assert( rc == 0 );
        ck_assert( queue_count( queue ) == i + 1 );
    }
    ck_assert( queue_count( queue ) == DEFAULT_CAPACITY );

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_remove( &queue );
        ck_assert( rc == 0 );

        if( i < DEFAULT_CAPACITY - 1 )
        {
            ck_assert( queue_count( queue ) == DEFAULT_CAPACITY - ( i + 1 ) );
            ck_assert( *(int *)queue_peek( queue ) == numbers[ i + 1 ] );
        }
        else
        {
            ck_assert( queue_count( queue ) == 0 );
            ck_assert( (int *)queue_peek( queue ) == NULL );
        }
    }
    ck_assert( queue_count( queue ) == 0 );
    ck_assert( (int *)queue_peek( queue ) == NULL );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( error_remove_empty_queue )
{
    queue_t * queue = NULL;
    int rc = -1;

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    rc = queue_remove( &queue );
    ck_assert( rc == -1 );
    ck_assert( error_code == E_UNDERFLOW );
    ck_assert( queue_count( queue ) == 0 );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( error_remove_null_queue )
{
    queue_t * queue = NULL;
    int rc = -1;

    rc = queue_remove( &queue );
    ck_assert( rc == -1 );
    ck_assert( error_code == E_NULL_PARAMETER );
}
END_TEST

START_TEST( remove_item_empty_queue )
{
    queue_t * queue = NULL;
    int rc = -1;
    int object = 42;

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    rc = queue_remove_item( &queue, &object );
    ck_assert( rc == -1 );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( remove_item_first )
{
    queue_t * queue = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = { 0 };
    int i = 0;

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_insert( &queue, &numbers[ i ] );
        ck_assert( rc == 0 );
        ck_assert( queue_count( queue ) == i + 1 );
    }
    ck_assert( queue_count( queue ) == DEFAULT_CAPACITY );

    rc = queue_remove_item( &queue, &numbers[ 0 ] );
    ck_assert( rc == 0 );
    ck_assert( queue_count( queue ) == DEFAULT_CAPACITY - 1 );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( remove_item_last )
{
    queue_t * queue = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = { 0 };
    int i = 0;

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_insert( &queue, &numbers[ i ] );
        ck_assert( rc == 0 );
        ck_assert( queue_count( queue ) == i + 1 );
    }
    ck_assert( queue_count( queue ) == DEFAULT_CAPACITY );

    rc = queue_remove_item( &queue, &numbers[ DEFAULT_CAPACITY - 1 ] );
    ck_assert( rc == 0 );
    ck_assert( queue_count( queue ) == DEFAULT_CAPACITY - 1 );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( remove_item_middle )
{
    queue_t * queue = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = { 0 };
    int i = 0;

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_insert( &queue, &numbers[ i ] );
        ck_assert( rc == 0 );
        ck_assert( queue_count( queue ) == i + 1 );
    }
    ck_assert( queue_count( queue ) == DEFAULT_CAPACITY );

    rc = queue_remove_item( &queue, &numbers[ DEFAULT_CAPACITY / 2 ] );
    ck_assert( rc == 0 );
    ck_assert( queue_count( queue ) == DEFAULT_CAPACITY - 1 );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( remove_item_in_queue )
{
    queue_t * queue = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = { 0 };
    int i = 0;

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_insert( &queue, &numbers[ i ] );
        ck_assert( rc == 0 );
        ck_assert( queue_count( queue ) == i + 1 );
    }
    ck_assert( queue_count( queue ) == DEFAULT_CAPACITY );

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_remove_item( &queue, &numbers[ i ] );
        ck_assert( rc == 0 );
        ck_assert( queue_count( queue ) == DEFAULT_CAPACITY - ( i + 1 ) );
    }
    ck_assert( queue_count( queue ) == 0 );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( remove_item_not_in_queue )
{
    queue_t * queue = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = { 0 };
    int other_numbers[ DEFAULT_CAPACITY ] = { 0 };
    int i = 0;

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
        other_numbers[ i ] = DEFAULT_CAPACITY + i + 1;
    }

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_insert( &queue, &numbers[ i ] );
        ck_assert( rc == 0 );
        ck_assert( queue_count( queue ) == i + 1 );
    }
    ck_assert( queue_count( queue ) == DEFAULT_CAPACITY );

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_remove_item( &queue, &other_numbers[ i ] );
        ck_assert( rc == -1 );
        ck_assert( queue_count( queue ) == DEFAULT_CAPACITY );
    }
    ck_assert( queue_count( queue ) == DEFAULT_CAPACITY );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( error_remove_item_null_queue )
{
    queue_t * queue = NULL;
    int rc = -1;
    int object = 42;

    rc = queue_remove_item( &queue, &object );
    ck_assert( rc == -1 );
    ck_assert( error_code == E_NULL_PARAMETER );
}
END_TEST

START_TEST( error_remove_item_null_object )
{
    queue_t * queue = NULL;
    int rc = -1;
    int * object = NULL;

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    rc = queue_remove_item( &queue, object );
    ck_assert( rc == -1 );
    ck_assert( error_code == E_NULL_PARAMETER );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( print_empty_queue )
{
    queue_t * queue = NULL;
    int rc = -1;

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    rc = queue_print( queue, stdout );
    ck_assert( rc > 0 );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
    
}
END_TEST

START_TEST( print_full_queue )
{
    queue_t * queue = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = { 0 };
    int i = 0;

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_insert( &queue, &numbers[ i ] );
        ck_assert( rc == 0 );
    }
    ck_assert( queue_count( queue ) == DEFAULT_CAPACITY );

    rc = queue_print( queue, stdout );
    ck_assert( rc > 0 );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( error_print_null_queue )
{
    queue_t * queue = NULL;
    int rc = -1;

    rc = queue_print( queue, stdout );
    ck_assert( rc == -1 );
    ck_assert( error_code == E_NULL_PARAMETER );
}
END_TEST

START_TEST( error_print_null_fp  )
{
    queue_t * queue = NULL;
    int rc = -1;

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    rc = queue_print( queue, NULL );
    ck_assert( rc == -1 );
    ck_assert( error_code == E_NULL_PARAMETER );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( memory_empty_queue )
{
    queue_t * queue = NULL;
    int rc = -1;

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    rc = queue_memory( queue );
    ck_assert( rc > 0 );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( memory_full_queue )
{
    queue_t * queue = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = { 0 };
    int i = 0;

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_insert( &queue, &numbers[ i ] );
        ck_assert( rc == 0 );
        ck_assert( queue_count( queue ) == i + 1 );
    }

    rc = queue_memory( queue );
    ck_assert( rc > 0 );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( error_memory_null_queue )
{
    queue_t * queue = NULL;
    int rc = -1;

    rc = queue_memory( queue );
    ck_assert( rc == -1 );
    ck_assert( error_code == E_NULL_PARAMETER );
}
END_TEST

START_TEST( count_empty_queue )
{
    queue_t * queue = NULL;
    int rc = -1;

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    rc = queue_count( queue );
    ck_assert( rc == 0 );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( count_full_queue )
{
    queue_t * queue = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = { 0 };
    int i = 0;

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_insert( &queue, &numbers[ i ] );
        ck_assert( rc == 0 );

        rc = queue_count( queue );
        ck_assert( rc == i + 1 );
    }

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( error_count_null_queue )
{
    queue_t * queue = NULL;
    int rc = -1;

    rc = queue_count( queue );
    ck_assert( rc == -1 );
    ck_assert( error_code == E_NULL_PARAMETER );
}
END_TEST

START_TEST( capacity_empty_queue )
{
    queue_t * queue = NULL;
    int rc = -1;

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    rc = queue_capacity( queue );
    ck_assert( rc == DEFAULT_CAPACITY );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( capacity_full_queue )
{
    queue_t * queue = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = { 0 };
    int i = 0;

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    ck_assert( queue_count( queue ) == 0 );

    rc = queue_capacity( queue );
    ck_assert( rc == DEFAULT_CAPACITY );

    for( i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_insert( &queue, &numbers[ i ] );
        ck_assert( rc == 0 );

        rc = queue_count( queue );
        ck_assert( rc == i + 1 );
    }

    rc = queue_capacity( queue );
    ck_assert( rc > 0 );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( error_capacity_null_queue )
{
    queue_t * queue = NULL;
    int rc = -1;

    rc = queue_capacity( queue );
    ck_assert( rc == -1 );
    ck_assert( error_code == E_NULL_PARAMETER );
}
END_TEST

/**
 * Create a test suite
 */
Suite * queue_suite( void )
{
    Suite * s = suite_create( "Queue" );

    /* core test cases */
    TCase * tc_core = tcase_create( "Core" );

    tcase_add_test( tc_core, create_and_destroy );
    tcase_add_test( tc_core, reserve_empty_queue );
    tcase_add_test( tc_core, reserve_full_queue );
    tcase_add_test( tc_core, reserve_decrease_size );
    tcase_add_test( tc_core, clear_empty_queue );
    tcase_add_test( tc_core, clear_full_queue );
    tcase_add_test( tc_core, peek_empty_queue );
    tcase_add_test( tc_core, peek_full_queue );
    tcase_add_test( tc_core, peek_remove_items_from_full_queue );
    tcase_add_test( tc_core, contains_empty_queue );
    tcase_add_test( tc_core, contains_in_queue );
    tcase_add_test( tc_core, contains_not_in_queue );
    tcase_add_test( tc_core, insert_empty_queue );
    tcase_add_test( tc_core, insert_objects );
    tcase_add_test( tc_core, remove_one_object );
    tcase_add_test( tc_core, remove_full_queue );
    tcase_add_test( tc_core, remove_item_empty_queue );
    tcase_add_test( tc_core, remove_item_first );
    tcase_add_test( tc_core, remove_item_last );
    tcase_add_test( tc_core, remove_item_middle );
    tcase_add_test( tc_core, remove_item_in_queue );
    tcase_add_test( tc_core, remove_item_not_in_queue );
    tcase_add_test( tc_core, print_empty_queue );
    tcase_add_test( tc_core, print_full_queue );
    tcase_add_test( tc_core, memory_empty_queue );
    tcase_add_test( tc_core, memory_full_queue );
    tcase_add_test( tc_core, count_empty_queue );
    tcase_add_test( tc_core, count_full_queue );
    tcase_add_test( tc_core, capacity_empty_queue );
    tcase_add_test( tc_core, capacity_full_queue );

    suite_add_tcase( s, tc_core );

    /* error test cases */
    TCase * tc_error = tcase_create( "Error" );

    tcase_add_test( tc_error, error_create_negative_capacity );
    tcase_add_test( tc_error, error_create_zero_capacity );
    tcase_add_test( tc_error, error_destroy_null_queue );
    tcase_add_test( tc_error, error_reserve_invalid_capacity );
    tcase_add_test( tc_error, error_reserve_null_queue );
    tcase_add_test( tc_error, error_clear_null_queue );
    tcase_add_test( tc_error, error_peek_null_queue );
    tcase_add_test( tc_error, error_contains_null_queue );
    tcase_add_test( tc_error, error_contains_null_object );
    tcase_add_test( tc_error, error_insert_full_queue );
    tcase_add_test( tc_error, error_insert_null_queue );
    tcase_add_test( tc_error, error_insert_null_object );
    tcase_add_test( tc_error, error_remove_empty_queue );
    tcase_add_test( tc_error, error_remove_null_queue );
    tcase_add_test( tc_error, error_remove_item_null_queue );
    tcase_add_test( tc_error, error_remove_item_null_object );
    tcase_add_test( tc_error, error_print_null_queue );
    tcase_add_test( tc_error, error_print_null_fp );
    tcase_add_test( tc_error, error_memory_null_queue );
    tcase_add_test( tc_error, error_count_null_queue );
    tcase_add_test( tc_error, error_capacity_null_queue );

    suite_add_tcase( s, tc_error );
    
    /* data tests */
    TCase * tc_data = tcase_create( "Data" );

    suite_add_tcase( s, tc_data );

    return s;
}

/**
 * Run test suite
 */
int main( void )
{
    int number_failed = 0;
    Suite * s = queue_suite();
    SRunner * sr = srunner_create( s );
    /* srunner_set_fork_status( sr, CK_NOFORK ); */
    srunner_run_all( sr, CK_NORMAL );
    number_failed = srunner_ntests_failed( sr );

    srunner_free( sr );
    
    if( number_failed == 0 )
    {
        return EXIT_SUCCESS;
    }
    else
    {
        return EXIT_FAILURE;
    }
}
