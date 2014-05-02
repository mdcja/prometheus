/**
 * \brief This file provides queue unit tests
 * \author Julian Martinez del Campo <mdcja@outlook.com>
 * \page queue_test Queue ADT Tests
 *
 * \section license GNU GPLv3
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * The attributes determine how the data is handled by queue. i.e. To properly
 * handle different data types such as integers or strings, the queue
 * attributes, or function pointers, must be set. 
 *
 * This file provides unit tests for the queue ADT. Unit tests are run using the
 * Check framework available at http://check.sourceforge.net/.
 *
 * \section queue_test_intro
 *
 * Due to the abstract nature of the queue being tested the implementation of a
 * queue may vary, therefore only black box tests will be considered. This
 * causes some problems with tests as the only way to properly test a given
 * function may depend on functions that may or may not work. However given the
 * amount of tests and the cases tested one can be confident that if the queue
 * passes all the tests, that it functions properly.
 *
 * \note If the queue is implemented with a linked list the test_capacity and
 * test_reserve tests will fail.
 */
#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <stdio.h>
#include <check.h>

#include "queue.h"
#include "attributes.h"
#include "error.h"

#define DEFAULT_CAPACITY 4 
#define SECONDARY_CAPACITY 200

#define MAX_STRLEN 256

/**
 * Compare two integers
 *
 * \param data1 an integer
 * \param data2 an integer
 * \return -1,0,1 if data1 is less than, equal to, or greater than data2
 */
int int_compare( const void * data1, const void * data2 )
{
    if( *(int *)data1 == *(int *)data2 )
    {
        return 0;
    }
    else if( *(int *)data1 < *(int *)data2 )
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

/**
 * Print an integer
 *
 * \param data an integer
 * \param fp a file pointer
 * \return the number of characters printed
 */
int int_print( const void * data, FILE * fp )
{
    return fprintf( fp, "%d", *(int *)data );
}

/**
 * Copy an integer
 */
void * int_copy( const void * data )
{
    int * new_num = NULL;

    new_num = (int *)calloc( 1, sizeof( int ) );
    if( new_num == NULL )
    {
        fprintf( stderr, "Error! could not allocate integer\n" );
        return NULL;
    }

    *new_num = *(int*)data;

    return new_num;
}

/**
 * Deallocate an integer
 */
void int_free( void * data )
{
    free( data );
}

/**
 * Compare a string
 */
int string_compare( const void * s1, const void * s2 )
{
    return strncmp( (const char *)s1, (const char *)s2, MAX_STRLEN );
}

/**
 * Print a string
 */
int string_print( const void * s, FILE * fp )
{
    return fprintf( fp, "(%p) %s", (void *)s, (const char*)s );
}

/**
 * Copy a string
 */
void * string_copy( const void * s )
{
    char * string = NULL;

    string = (char *)calloc( MAX_STRLEN, sizeof( char ) );
    if( string == NULL )
    {
        fprintf( stderr, "Error! could not allocate string\n" );
        return NULL;
    }
    strncpy( string, (const char *)s, MAX_STRLEN );

    return string;
}

/**
 * Free a string
 */
void string_free( void * s )
{
    free( s );
}

/**
 * Test creation and deletion of a queue
 */

/**
 * Test the creation/destruction of a queue
 *
 * A queue is created with NULL attributes, and a valid capacity. The queue is
 * then destroyed.
 *
 * \post No error occurs
 */
START_TEST( test_create_destroy_default_params )
{
    queue_t * queue = NULL;
    int rc = -1;

    /* create a queue correctly */
    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    /* destroy a queue */
    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
    ck_assert( queue == NULL );
}
END_TEST

/**
 * Test the invalid creation/destruction of a queue
 *
 * A queue is created with NULL attribues, and an invalid capacity of 0.
 * The queue cannot be destroyed because it was not created.
 *
 * \post An error (ERROR_INVALID_CAPACITY) occurs in queue_create().
 * \post An error (ERROR_NULL_PARAMETER) occurs in queue_destroy().
 */
START_TEST( error_test_create_destroy_null_attr_zero_capacity )
{
    queue_t * queue = NULL;
    int rc = -1;

    /* create a queue with zero capacity */
    queue = queue_create( NULL, 0 );
    ck_assert( queue == NULL );
    ck_assert( error_code == ERROR_INVALID_CAPACITY );

    /* destroy null queue */
    rc = queue_destroy( &queue );
    ck_assert( rc == -1 );
    ck_assert( error_code == ERROR_NULL_PARAMETER );
    ck_assert( queue == NULL );
}
END_TEST

/**
 * Test the invalid creation/destruction of a queue
 *
 * A queue is created with NULL attributes, and an invalid negative capacity.
 * The queue cannot be destroyed because it was not created.
 *
 * \post An error (ERROR_INVALID_CAPACITY) occurs in queue_create().
 * \post An error (ERROR_NULL_PARAMETER) occurs in queue_destroy().
 */
START_TEST( error_test_create_destroy_null_attr_negative_capacity )
{
    queue_t * queue = NULL;
    int rc = -1;

    /* create a queue with negative capacity */
    queue = queue_create( NULL, -10 );
    ck_assert( queue == NULL );
    ck_assert( error_code == ERROR_INVALID_CAPACITY );

    /* destroy null queue */
    rc = queue_destroy( &queue );
    ck_assert( rc == -1 );
    ck_assert( error_code == ERROR_NULL_PARAMETER );
    ck_assert( queue == NULL );
}
END_TEST

/**
 * Test the creation/destruction of a queue
 *
 * A queue is created with attributes, and a valid capacity. The queue is then
 * destroyed.
 *
 * \post No errors occur.
 */
START_TEST( test_create_destroy_attr )
{
    queue_t * queue = NULL;
    int rc = -1;
    attr_t attributes = {0};

    /* initialize attributes */
    attr_init( &attributes );

    /* create a queue with attributes and a default capacity */
    queue = queue_create( &attributes, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    /* destroy queue */
    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
    ck_assert( queue == NULL );
}
END_TEST

/**
 * Test the invalid creation/destruction of a queue
 *
 * A queue is created with attributes, and an invalid zero capacity. The queue
 * cannot be destroyed because it was not created.
 *
 * \post An error (ERROR_INVALID_CAPACITY) occurs in queue_create().
 * \post An error (ERROR_NULL_PARAMETER) occurs in queue_destroy().
 */
START_TEST( error_test_create_destroy_attr_zero_capacity )
{
    queue_t * queue = NULL;
    int rc = -1;
    attr_t attributes = {0};

    /* initialize attributes */
    attr_init( &attributes );

    /* create a queue with zero capacity */
    queue = queue_create( &attributes, 0 );
    ck_assert( queue == NULL );
    ck_assert( error_code == ERROR_INVALID_CAPACITY );

    /* destroy null queue */
    rc = queue_destroy( &queue );
    ck_assert( rc == -1 );
    ck_assert( error_code == ERROR_NULL_PARAMETER );
    ck_assert( queue == NULL );
}
END_TEST

/**
 * Test the invalid creation/destruction of a queue
 *
 * A queue is created with attributes, and an invalid negative capacity. The
 * queue cannot be destroyed because it was never created.
 *
 * \post An error (ERROR_INVALID_CAPACITY) occurs in queue_create().
 * \post An error (ERROR_NULL_PARAMETER) occurs in queue_destroy().
 */
START_TEST( error_test_create_destroy_attr_negative_capacity )
{
    queue_t * queue = NULL;
    int rc = -1;
    attr_t attributes = {0};

    /* initialize attributes */
    attr_init( &attributes );

    /* create queue with negative capacity */
    queue = queue_create( &attributes, -1 );
    ck_assert( queue == NULL );
    ck_assert( error_code == ERROR_INVALID_CAPACITY );

    /* destroy null queue */
    rc = queue_destroy( &queue );
    ck_assert( rc == -1 );
    ck_assert( error_code == ERROR_NULL_PARAMETER );
    ck_assert( queue == NULL );
}
END_TEST

/**
 * Test push/insert
 */

/**
 * Test the insertion of one item
 *
 * A queue is created with NULL attributes and a valid capacity, a number is
 * then added to the queue. The queue is then destroyed.
 *
 * \post No errors occur.
 */
START_TEST( test_insert_one_items )
{
    queue_t * queue = NULL;
    int count = 0;
    int rc = -1;
    int number0 = 0;

    /* create queue */
    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    /* insert item into queue */
    rc = queue_insert( &queue, &number0 );
    ck_assert( rc == 0 );

    /* check number of items in the queue */
    count = queue_count( queue );
    ck_assert( count == 1 );

    /* destroy queue */
    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
    ck_assert( queue == NULL );
}
END_TEST

/**
 * Test the insertion of two items
 *
 * A queue is created with NULL attributes and a valid capacity, a number is
 * then added to the queue twice. The queue is the destroyed.
 *
 * \post No errors occur.
 */
START_TEST( test_insert_two_items )
{
    queue_t * queue = NULL;
    int count = 0;
    int rc = -1;
    int number0 = 0;

    /* create queue */
    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    /* add two items */
    for( int i = 0; i < 2; ++i )
    {
        rc = queue_insert( &queue, &number0 );
        ck_assert( rc == 0 );

        count = queue_count( queue );
        ck_assert( count == i + 1 );
    }

    /* destroy queue */
    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
    ck_assert( queue == NULL );
}
END_TEST

/**
 * Test the insertion of three items
 *
 * A queue is created with NULL attributes and a valid capacity, a number is
 * then added to the queue three times. The queue is then destroyed.
 *
 * \post No errors occur.
 */
START_TEST( test_insert_three_items )
{
    queue_t * queue = NULL;
    int count = 0;
    int rc = -1;
    int number0 = 0;

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    /* insert three items */
    for( int i = 0; i < 3; ++i )
    {
        rc = queue_insert( &queue, &number0 );
        ck_assert( rc == 0 );

        count = queue_count( queue );
        ck_assert( count == i + 1 );
    }

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
    ck_assert( queue == NULL );
}
END_TEST

/**
 * Test the insertion of items to capacity
 *
 * A queue is created with NULL attributes and a valid capacity, a number is
 * then added to the queue to capacity. The queue is then destroyed.
 *
 * \post No errors occur.
 */
START_TEST( test_insert_capacity )
{
    queue_t * queue = NULL;
    int count = 0;
    int rc = -1;
    int number[ DEFAULT_CAPACITY ] = {0};
    int * n = NULL;

    /* populate test numbers */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        number[ i ] = i + 1;
    }

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    /* insert items to capacity */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_insert( &queue, &number[ i ] );
        ck_assert( rc == 0 );

        /* check number of items */
        count = queue_count( queue );
        ck_assert( count == i + 1 );

        /* check that front of queue is 0 */
        n = queue_peek( queue );
        ck_assert( n != NULL );
        ck_assert( *n = 0 );
    }

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
    ck_assert( queue == NULL );
}
END_TEST

/**
 * Test the invalid insertion of items
 *
 * A queue is created, then filled to capacity, an item is then inserted into
 * the queue. The queue is full, so there should be an error. The queue is then
 * destroyed.
 *
 * \post No errors occur.
 */
START_TEST( error_test_insert_over_capacity )
{
    queue_t * queue = NULL;
    int count = 0;
    int rc = -1;
    int number[ DEFAULT_CAPACITY ] = {0};

    /* populate test numbers */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        number[ i ] = i + 1;
    }

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    /* insert items to capacity */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_insert( &queue, &number[ i ] );
        ck_assert( rc == 0 );
    }

    /* insert over capacity */
    rc = queue_insert( &queue, &number[0] );
    ck_assert( rc == -1 );
    ck_assert( error_code == ERROR_OVERFLOW );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
    ck_assert( queue == NULL );
}
END_TEST

/**
 * Test the invalid insertion of an item
 *
 * A queue is not created, and as a result nothing can be inserted into the
 * queue and an error occurs.
 *
 * \post An error (ERROR_NULL_PARAMETER) occurs in queue_insert().
 */
START_TEST( error_test_insert_null_queue )
{
    queue_t * queue = NULL;
    int rc = -1;
    int number0 = 0;

    /* try to add an item */
    rc = queue_insert( &queue, &number0 );
    ck_assert( rc == -1 );
    ck_assert( error_code == ERROR_NULL_PARAMETER );
}
END_TEST

/**
 * Test the invalid insertion of an item
 *
 * A queue is created with NULL attributes and a valid capacity. An item with
 * NULL value is then inserted into the queue, resulting in an error. The queue
 * is then destroyed.
 *
 * \post An error (ERROR_NULL_PARAMETER) occurs in queue_insert().
 */
START_TEST( error_test_insert_null_item )
{
    queue_t * queue = NULL;
    int * number = NULL;
    int rc = -1;

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    /* insert null item */
    rc = queue_insert( &queue, number );
    ck_assert( rc == -1 );
    ck_assert( error_code == ERROR_NULL_PARAMETER );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
    ck_assert( queue == NULL );
}
END_TEST

/**
 * Thest the invalid insertion of an item
 *
 * A queue is not created, and an item iwth NULL value is inserted into the
 * queue, resulting in an error.
 *
 * \post An error (ERROR_NULL_PARAMETER) occurs in queue_insert().
 */
START_TEST( error_test_insert_null_queue_null_item )
{
    queue_t * queue = NULL;
    int rc = -1;
    int * number = NULL;

    /* insert NULL item into NULL queue */
    rc = queue_insert( &queue, number );
    ck_assert( rc == -1 );
    ck_assert( error_code == ERROR_NULL_PARAMETER );
}
END_TEST

/**
 * Test insertion of items into a queue
 *
 * A queue is created with NULL attributes, and items are then added to the
 * queue. The order and count of items in the queue is checked, as well as
 * inserting items past capacity. The queue is then destroyed.
 *
 * \post An error (ERROR_OVERFLOW) occurs in queue_insert().
 * \post Insertion of items into a queue are confirmed.
 */
START_TEST( test_insert )
{
    queue_t * queue = NULL;
    int rc = -1;
    int count = -1;
    int numbers[ DEFAULT_CAPACITY ] = {0};
    int * n = NULL;

    error_code = 0; /* init error */
    
    /* initialize test values */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    /* create queue */
    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    /* insert items */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_insert( &queue, &numbers[ i ] );
        ck_assert( rc == 0 );

        /* check insertion */
        count = queue_count( queue );
        ck_assert( count == i + 1 );

        n = (int *)queue_peek( queue );
        ck_assert( n != NULL );
        ck_assert( *n == numbers[ 0 ] );
    }

    /* insert item into full queue */
    rc = queue_insert( &queue, &numbers[ 0 ] );

    /* check for error and type of error */
    if( rc == -1 )
    {
        ck_assert( error_code == ERROR_OVERFLOW );
    }
    else
    {
        /* using a linked list */
        ck_assert( error_code == 0 );
    }

    /* destroy queue */
    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test remove/pop
 */

/**
 * Test invalid removal of item
 *
 * A queue is created with NULL attributes and a valid capacity, then an item
 * is removed. This results in an error, as there are not items in the queue.
 * The queue is then destroyed.
 *
 * \post An error (ERROR_UNDERFLOW) occurs in queue_remove().
 */
START_TEST( error_test_remove_empty )
{
    queue_t * queue = NULL;
    int rc = -1;
    int number0 = 0;
    
    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    /* remove item from empty queue */
    rc = queue_remove( &queue );
    ck_assert( rc == -1 );
    ck_assert( error_code == ERROR_UNDERFLOW );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test removal of item
 *
 * A queue is created with NULL attributes, and an item is inserted, an item is
 * then removed from the queue. The queue is then destroyed.
 *
 * \post No errors occur.
 */
START_TEST( test_remove_one )
{
    queue_t * queue = NULL;
    int rc = -1;
    int number0 = 0;
    
    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    rc = queue_insert( &queue, &number0 );
    ck_assert( rc == 0 );

    /* remove item */
    rc = queue_remove( &queue );
    ck_assert( rc == 0 );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test removal of items
 *
 * A queue is created with NULL attributes, and two items are inserted into the
 * queue. The items are then removed from the queue, and the queue is
 * destroyed.
 *
 * \post No errors occur.
 */
START_TEST( test_remove_two )
{
    queue_t * queue = NULL;
    int rc = -1;
    int number0 = 0;
    
    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    for( int i = 0; i < 2; ++i )
    {
        rc = queue_insert( &queue, &number0 );
        ck_assert( rc == 0 );
    }

    /* remove */
    for( int i = 0; i < 2; ++i )
    {
        rc = queue_remove( &queue );
        ck_assert( rc == 0 );
    }

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test removal of items
 *
 * A queue is created with NULL attributes, and three items are inserted into
 * the queue. The items are then removed from the queue, and the queue is
 * destroyed.
 *
 * \post No errors occur.
 */
START_TEST( test_remove_three )
{
    queue_t * queue = NULL;
    int rc = -1;
    int number0 = 0;
    
    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    for( int i = 0; i < 3; ++i )
    {
        rc = queue_insert( &queue, &number0 );
        ck_assert( rc == 0 );
    }

    /* remove */
    for( int i = 0; i < 3; ++i )
    {
        rc = queue_remove( &queue );
        ck_assert( rc == 0 );
    }

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test removal of items
 *
 * A queue is created with NULL attributes, and filled to capacity. All of the
 * items are then removed from the queue, and the queue is destroyed.
 *
 * \post No errors occur.
 */
START_TEST( test_remove_all )
{
    queue_t * queue = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = {0};

    /* initialize numbers */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_insert( &queue, &numbers[ i ] );
        ck_assert( rc == 0 );
    }

    /* remove items */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_remove( &queue );
        ck_assert( rc == 0 );
    }

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test invalid removal of items
 *
 * A queue is NOT created and queue_remove() is run on the NULL queue.
 *
 * \post An error (ERROR_NULL_PARAMETER) occurs.
 */
START_TEST( error_test_remove_null_queue )
{
    queue_t * queue = NULL;
    int rc = -1;

    error_code = 0; /* init error code */

    rc = queue_remove( &queue );
    ck_assert( rc == -1 );
    ck_assert( error_code == ERROR_NULL_PARAMETER );
}
END_TEST

/**
 * Test removal of items
 *
 * \post The removal of items is checked.
 */
START_TEST( test_remove )
{
    queue_t * queue = NULL;
    int rc = -1;
    int count = -1;
    int numbers[ DEFAULT_CAPACITY ] = {0};
    int * n = NULL;


    /* initialize test values
     */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1; 
    }

    /* create queue */
    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    /* remove from empty queue */
    error_code = 0;
    rc = queue_remove( &queue );
    ck_assert( rc == -1 );
    ck_assert( error_code == ERROR_UNDERFLOW );

    /* insert items */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_insert( &queue, &numbers[ i ] );
        ck_assert( rc == 0 );
    }

    /* remove items */
    for( int i = DEFAULT_CAPACITY; i > 0; --i )
    {
        rc = queue_remove( &queue );
        ck_assert( rc == 0 );

        /* check removal */
        count = queue_count( queue );
        ck_assert( count == i - 1 );

        if( i > 1 )
        {
            n = (int *)queue_peek( queue );
            ck_assert( n != NULL );
            ck_assert( *n == numbers[ DEFAULT_CAPACITY - ( i - 1 )] );
        }
        else 
        {
            n = (int *)queue_peek( queue );
            ck_assert( n == NULL );
        }
    }

    /* destroy queue */
    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test clear
 */

/**
 * Clear a queue
 *
 * A queue is created with NULL attributes and cleared, the queue is then destroyed.
 *
 * \post No errors occur.
 */
START_TEST( test_clear_empty )
{
    queue_t * queue = NULL;
    int rc = -1;

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    /* clear the queue */
    rc = queue_clear( &queue );
    ck_assert( rc == 0 );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Clear a queue with one item
 *
 * A queue is created with NULL attributes and an item is inserted into the
 * queue. The queue is then cleared, and then destroyed.
 *
 * \post No errors occur.
 */
START_TEST( test_clear_one )
{
    queue_t * queue = NULL;
    int rc = -1;
    int number = 0;

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    rc = queue_insert( &queue, &number );
    ck_assert( rc == 0 );

    rc = queue_clear( &queue );
    ck_assert( rc == 0 );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( test_clear_capacity )
{
}
END_TEST

START_TEST( error_test_clear_null_queue )
{
}
END_TEST

START_TEST( test_clear )
{
    queue_t * queue = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = {0};

    /* initialize test values
     */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    /* create queue */
    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    /* test clear on empty queue 
     */
    rc = queue_clear( &queue );
    ck_assert( rc == 0 );

    /* insert items */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_insert( &queue, &numbers[ i ] );
        ck_assert( rc == 0 );
    }

    /* test clear on full list
     */
    rc = queue_clear( &queue );
    ck_assert( rc == 0 );
    ck_assert( queue_count( queue ) == 0 );

    /* destroy queue */
    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test memory usage
 */
START_TEST( test_memory )
{
    queue_t * queue = NULL;
    int rc = -1;
    int memory_usage = -1;

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    /* test memory usage */
    memory_usage = queue_memory( queue );
    ck_assert( memory_usage > 0 );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test capacity
 */
START_TEST( test_capacity_primary_default )
{
}
END_TEST

START_TEST( test_capacity_secondary_default )
{
}
END_TEST

START_TEST( error_test_capacity_null_param )
{
}
END_TEST

START_TEST( test_capacity )
{
    queue_t * queue = NULL;
    int rc = -1;
    int capacity = -1;

    error_code = 0;

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    /* get capacity */
    capacity = queue_capacity( queue );
    ck_assert( capacity == DEFAULT_CAPACITY );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test peek
 */
START_TEST( test_peek_empty )
{
    queue_t * queue = NULL;
    int rc = -1;
    int * n = NULL;

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    /* test with empty queue */
    n = (int *)queue_peek( queue );
    ck_assert( n == NULL );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( test_peek_one_item )
{
    queue_t * queue = NULL;
    int rc = -1;
    int number = 128;
    int * n = NULL;

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    /* test with item in queue */
    rc = queue_insert( &queue, &number );
    ck_assert( rc == 0 );

    n = (int *)queue_peek( queue );
    ck_assert( n != NULL );
    ck_assert( *n == number );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( test_peek_two_items )
{
    queue_t * queue = NULL;
    int rc = -1;
    int number0 = 128;
    int number1 = 256;
    int * n = NULL;

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    rc = queue_insert( &queue, &number0 );
    ck_assert( rc == 0 );

    rc = queue_insert( &queue, &number1 );
    ck_assert( rc == 0 );

    /* test with item in queue */
    n = (int *)queue_peek( queue );
    ck_assert( n != NULL );
    ck_assert( *n == number0 );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( test_peek_three_items )
{
    queue_t * queue = NULL;
    int rc = -1;
    int number0 = 128;
    int number1 = 256;
    int number2 = 512;
    int * n = NULL;

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    rc = queue_insert( &queue, &number0 );
    ck_assert( rc == 0 );

    rc = queue_insert( &queue, &number1 );
    ck_assert( rc == 0 );

    rc = queue_insert( &queue, &number2 );
    ck_assert( rc == 0 );

    /* test with item in queue */
    n = (int *)queue_peek( queue );
    ck_assert( n != NULL );
    ck_assert( *n == number0 );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( error_test_peek_null_queue )
{
    queue_t * queue = NULL;
    int rc = -1;
    int number = 128;
    int * n = NULL;

    error_code = 0;

    /* test with item in queue */
    n = (int *)queue_peek( queue );
    ck_assert( n == NULL );
    ck_assert( error_code == ERROR_NULL_PARAMETER );

    rc = queue_destroy( &queue );
    ck_assert( rc == -1 );
}
END_TEST

/**
 * Test count
 */
START_TEST( test_count_empty )
{
    queue_t * queue = NULL;
    int rc = -1;
    int number = 1;

    /* create queue */
    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    rc = queue_count( queue );
    ck_assert( rc == 0 );

    /* destroy queue */
    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( test_count_one_item )
{
}
END_TEST

START_TEST( test_count_two_items )
{
}
END_TEST

START_TEST( test_count_three_items )
{
}
END_TEST

START_TEST( test_count_capacity )
{
}
END_TEST

START_TEST( test_count_over_capacity )
{
}
END_TEST

START_TEST( error_test_count_null_param )
{
}
END_TEST

START_TEST( test_count )
{
    queue_t * queue = NULL;
    int rc = -1;
    int count = -1;
    int numbers[ DEFAULT_CAPACITY ] = {0};

    /* initialize test values
     */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    /* create queue */
    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    /* get count on empty queue */
    count = queue_count( queue );
    ck_assert( count == 0 );

    /* insert items and check count */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_push( &queue, &numbers[ i ] );
        ck_assert( rc == 0 );

        count = queue_count( queue );
        ck_assert( count == i + 1 );
    }

    /* remove items and check count */
    for( int i = DEFAULT_CAPACITY; i > 0; --i )
    {
        rc = queue_pop( &queue );
        ck_assert( rc == 0 );

        count = queue_count( queue );
        ck_assert( count = DEFAULT_CAPACITY - 1 );
    }

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test reserve
 */
START_TEST( test_reserve_empty )
{
}
END_TEST

START_TEST( test_reserve_capacity )
{
}
END_TEST

START_TEST( test_reserve_same_capacity )
{
}
END_TEST

START_TEST( error_test_reserve_negative_capacity )
{
}
END_TEST

START_TEST( error_test_reserve_zero_capacity )
{
}
END_TEST

START_TEST( error_test_reserve_capacity_too_small )
{
}
END_TEST

START_TEST( error_test_reserve_null_queue )
{
}
END_TEST

START_TEST( test_reserve )
{
    queue_t * queue = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = {0};

    /* initialize test values */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );
    
    /* resize an empty queue */
    ck_assert( queue_capacity( queue ) == DEFAULT_CAPACITY );

    rc = queue_reserve( &queue, SECONDARY_CAPACITY );
    ck_assert( rc == 0 );
    ck_assert( queue_capacity( queue ) == SECONDARY_CAPACITY );

    /* resize back to default capacity */
    rc = queue_reserve( &queue, DEFAULT_CAPACITY );
    ck_assert( rc == 0 );
    ck_assert( queue_capacity( queue ) == DEFAULT_CAPACITY );

    /* add items to queue and resize again */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_push( &queue, &numbers[ i ] );
        ck_assert( rc == 0 );
    }

    rc = queue_reserve( &queue, SECONDARY_CAPACITY );
    ck_assert( rc == 0 );
    ck_assert( queue_capacity( queue ) ==  SECONDARY_CAPACITY );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test contains
 */
START_TEST( test_contains_true )
{
}
END_TEST

START_TEST( test_contains_false )
{
}
END_TEST

START_TEST( test_contains_empty )
{
}
END_TEST

START_TEST( error_test_contains_null_queue )
{
}
END_TEST

START_TEST( error_test_contains_null_param )
{
}
END_TEST

START_TEST( test_contains )
{
    queue_t * queue = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = {0};
    int z = -5;

    /* initialize test values */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    /* test emtpy queue */
    rc = queue_contains( queue, &z );
    ck_assert( rc == 0 );

    /* add data to queue */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_push( &queue, &numbers[ i ] );
        ck_assert( rc == 0 );
    }

    /* look for data NOT in queue */
    rc = queue_contains( queue, &z );
    ck_assert( rc == 0 );

    /* look for data in queue */
    rc = queue_contains( queue, &numbers[0] );
    ck_assert( rc == 1 );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test remove item
 */
START_TEST( test_remove_item_empty )
{
}
END_TEST

START_TEST( test_remove_item_true )
{
}
END_TEST

START_TEST( test_remove_item_true_one_item )
{
}
END_TEST

START_TEST( test_remove_item_true_two_items )
{
}
END_TEST

START_TEST( test_remove_item_false )
{
}
END_TEST

START_TEST( error_test_remove_item_null_queue )
{
}
END_TEST

START_TEST( test_remove_item )
{
    queue_t * queue = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = {0};
    int z = -5;

    /* initialize test values */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    /* remove item from empty queue */
    error_code = 0;
    rc = queue_remove_item( &queue, &z );
    ck_assert( rc == -1 );
    ck_assert( error_code == ERROR_NOT_FOUND );

    /* add data to queue */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_push( &queue, &numbers[ i ] );
        ck_assert( rc == 0 );
    }

    /* remove nonexistent item from queue */
    error_code = 0;
    rc = queue_remove_item( &queue, &z );
    ck_assert( rc == -1 );
    ck_assert( error_code == ERROR_NOT_FOUND );

    /* remove item from queue */
    rc = queue_remove_item( &queue, &numbers[ 0 ] );
    ck_assert( rc == 0 );
    ck_assert( queue_count( queue ) == DEFAULT_CAPACITY - 1 );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test print
 */
START_TEST( test_print_empty )
{
}
END_TEST

START_TEST( test_print_capacity )
{
}
END_TEST

START_TEST( error_test_print_null_queue )
{
}
END_TEST

START_TEST( error_test_print_null_fp )
{
}
END_TEST

START_TEST( test_print )
{
    queue_t * queue = NULL;
    int rc = 0;
    int nchr = -1;
    int numbers[ DEFAULT_CAPACITY ] = {0};

    /* initialize test values
     */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    /* print empty queue */
    nchr = queue_print( queue, stdout );
    ck_assert( nchr > 0 );

    /* insert items and print queue */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_insert( &queue, &numbers[ i ] );
        ck_assert( rc == 0 );
    }

    nchr = queue_print( queue, stdout );
    ck_assert( nchr > 0 );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test print integer
 */
START_TEST( test_print_int )
{
    queue_t * queue = NULL;
    attr_t attributes = {0};
    int rc = 0;
    int nchr = -1;
    int numbers[ DEFAULT_CAPACITY ] = {0};

    /* initialize test values */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    /* set integer data */
    attr_init( &attributes );
    attr_set_compare( &attributes, int_compare );
    attr_set_print( &attributes, int_print );
    attr_set_copy( &attributes, int_copy );
    attr_set_free( &attributes, int_free );

    queue = queue_create( &attributes, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    /* print empty queue */
    nchr = queue_print( queue, stdout );
    ck_assert( nchr > 0 );

    /* insert items and print queue */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_insert( &queue, &numbers[ i ] );
        ck_assert( rc == 0 );
    }

    nchr = queue_print( queue, stdout );
    ck_assert( nchr > 0 );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test string print
 */
START_TEST( test_print_string )
{
    queue_t * queue = NULL;
    attr_t attributes = {0};
    int rc = -1;
    int nchr = -1;
    /** 
     * The first 101 words of the poem "Kubla Khan" by Samuel Taylor Coleridge
     *
     * \see http://www.poetryfoundation.org/poem/173247
     */
    char * words[ MAX_STRLEN ] = { "In", "Xanadu", "did", "Kubla", "Khan", "A",
    "stately", "pleasure-dome", "decree:", "Where", "Alph,", "the", "sacred",
    "river,", "ran", "Through", "caverns", "measureless", "to", "man", "Down",
    "to", "a", "sunless", "sea.",  "So", "twice", "five", "miles", "of",
    "fertile", "ground", "With", "walls", "and", "towers", "were", "girdled",
    "round;", "And", "there", "were", "gardens", "bright", "with", "sinuous",
    "rills,", "Where", "blossomed", "many", "an", "incense-bearing", "tree;",
    "And", "here", "were", "forests", "ancient", "as", "the", "hills,",
    "Enfolding", "sunny", "spots", "of", "greenery.",  "But", "oh!", "that",
    "deep", "romantic", "chasm", "which", "slanted", "Down", "the", "green",
    "hill", "athwart", "a", "cedarn", "cover!", "A", "savage", "place!", "as",
    "holy", "and", "enchanted", "As", "e’er", "beneath", "a", "waning", "moon",
    "was", "haunted", "By", "woman", "wailing", "for", "her", "demon-lover!",
    "And", "from", "this", "chasm,", "with", "ceaseless", "turmoil", "seething"
    };

    /* set integer data */
    attr_init( &attributes );
    attr_set_compare( &attributes, string_compare );
    attr_set_print( &attributes, string_print );
    attr_set_copy( &attributes, string_copy );
    attr_set_free( &attributes, string_free );

    queue = queue_create( &attributes, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    /* print empty queue */
    nchr = queue_print( queue, stdout );
    ck_assert( nchr > 0 );

    /* insert items and print queue */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_insert( &queue, words[ i ] );
        ck_assert( rc == 0 );
    }

    nchr = queue_print( queue, stdout );
    ck_assert( nchr > 0 );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test contains with a string
 */
START_TEST( test_contains_string )
{
    queue_t * queue = NULL;
    attr_t attributes = {0};
    int rc = -1;

    /** 
     * The first 101 words of the poem "Kubla Khan" by Samuel Taylor Coleridge
     *
     * \see http://www.poetryfoundation.org/poem/173247
     */
    char * words[ MAX_STRLEN ] = { "In", "Xanadu", "did", "Kubla", "Khan", "A",
    "stately", "pleasure-dome", "decree:", "Where", "Alph,", "the", "sacred",
    "river,", "ran", "Through", "caverns", "measureless", "to", "man", "Down",
    "to", "a", "sunless", "sea.",  "So", "twice", "five", "miles", "of",
    "fertile", "ground", "With", "walls", "and", "towers", "were", "girdled",
    "round;", "And", "there", "were", "gardens", "bright", "with", "sinuous",
    "rills,", "Where", "blossomed", "many", "an", "incense-bearing", "tree;",
    "And", "here", "were", "forests", "ancient", "as", "the", "hills,",
    "Enfolding", "sunny", "spots", "of", "greenery.",  "But", "oh!", "that",
    "deep", "romantic", "chasm", "which", "slanted", "Down", "the", "green",
    "hill", "athwart", "a", "cedarn", "cover!", "A", "savage", "place!", "as",
    "holy", "and", "enchanted", "As", "e’er", "beneath", "a", "waning", "moon",
    "was", "haunted", "By", "woman", "wailing", "for", "her", "demon-lover!",
    "And", "from", "this", "chasm,", "with", "ceaseless", "turmoil", "seething"
    };

    /* set integer data */
    attr_init( &attributes );
    attr_set_compare( &attributes, string_compare );
    attr_set_print( &attributes, string_print );
    attr_set_copy( &attributes, string_copy );
    attr_set_free( &attributes, string_free );

    queue = queue_create( &attributes, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    /* insert items and print queue */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = queue_insert( &queue, words[ i ] );
        ck_assert( rc == 0 );
    }

    /* check for words */
    rc = queue_contains( queue, "sunless" );
    ck_assert( rc == 1 );

    rc = queue_contains( queue, "enchanted" );
    ck_assert( rc == 1 );

    /* not in queue */
    rc = queue_contains( queue, "foo" );
    ck_assert( rc == 0 );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

START_TEST( test_create_destroy_int )
{
    queue_t * queue = NULL;
    int rc = -1;
    attr_t int_attr = {0};

    attr_init( &int_attr );
    attr_set_compare( &int_attr, int_compare );
    attr_set_print( &int_attr, int_print );
    attr_set_free( &int_attr, int_free );
    attr_set_copy( &int_attr, int_copy );

    queue = queue_create( &int_attr, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
    ck_assert( queue == NULL );
}
END_TEST

START_TEST( test_create_destroy_string )
{
    queue_t * queue = NULL;
    int rc = -1;
    attr_t string_attr = {0};

    attr_init( &string_attr );
    attr_set_compare( &string_attr, string_compare );
    attr_set_print( &string_attr, string_print );
    attr_set_free( &string_attr, string_free );
    attr_set_copy( &string_attr, string_copy );

    queue = queue_create( &string_attr, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
    ck_assert( queue == NULL );
}
END_TEST

/**
 * Create a test suite
 */
Suite * queue_suite( void )
{
    Suite * s = suite_create( "Queue" );
    TCase * tc_core = tcase_create( "Core" );
    TCase * tc_data = tcase_create( "Data" );
    TCase * tc_error = tcase_create( "Error" );

    /* core test cases */
    tcase_add_test( tc_core, test_create_destroy_default_params );
    tcase_add_test( tc_core, test_create_destroy_attr );
    tcase_add_test( tc_core, test_insert_one_items );
    tcase_add_test( tc_core, test_insert_two_items );
    tcase_add_test( tc_core, test_insert_three_items );
    tcase_add_test( tc_core, test_insert_capacity );
    tcase_add_test( tc_core, test_remove_one );
    tcase_add_test( tc_core, test_remove_two );
    tcase_add_test( tc_core, test_remove_three );
    tcase_add_test( tc_core, test_remove_all );
    tcase_add_test( tc_core, test_clear_empty );
    tcase_add_test( tc_core, test_clear_capacity );
    tcase_add_test( tc_core, test_peek_empty );
    tcase_add_test( tc_core, test_peek_one_item );
    tcase_add_test( tc_core, test_peek_two_items );
    tcase_add_test( tc_core, test_peek_three_items );
    tcase_add_test( tc_core, test_count_empty );
    tcase_add_test( tc_core, test_count_one_item );
    tcase_add_test( tc_core, test_count_two_items );
    tcase_add_test( tc_core, test_count_three_items );
    tcase_add_test( tc_core, test_count_capacity );
    tcase_add_test( tc_core, test_count_over_capacity );
    tcase_add_test( tc_core, test_capacity_primary_default );
    tcase_add_test( tc_core, test_capacity_secondary_default );
    tcase_add_test( tc_core, test_reserve_empty );
    tcase_add_test( tc_core, test_reserve_capacity );
    tcase_add_test( tc_core, test_reserve_same_capacity );
    tcase_add_test( tc_core, test_contains_true );
    tcase_add_test( tc_core, test_contains_false );
    tcase_add_test( tc_core, test_contains_empty );
    tcase_add_test( tc_core, test_remove_item_empty );
    tcase_add_test( tc_core, test_remove_item_true );
    tcase_add_test( tc_core, test_remove_item_true_one_item );
    tcase_add_test( tc_core, test_remove_item_true_two_items );
    tcase_add_test( tc_core, test_remove_item_false );
    tcase_add_test( tc_core, test_print_empty );
    tcase_add_test( tc_core, test_print_capacity );


    tcase_add_test( tc_core, test_insert );
    tcase_add_test( tc_core, test_remove );
    tcase_add_test( tc_core, test_clear );
    tcase_add_test( tc_core, test_memory );
    tcase_add_test( tc_core, test_capacity );
    tcase_add_test( tc_core, test_count );
    tcase_add_test( tc_core, test_reserve );
    tcase_add_test( tc_core, test_contains );
    tcase_add_test( tc_core, test_remove_item );
    tcase_add_test( tc_core, test_print );

    /* data tests */
    tcase_add_test( tc_data, test_create_destroy_int );
    tcase_add_test( tc_data, test_create_destroy_string );
    tcase_add_test( tc_data, test_print_int );
    tcase_add_test( tc_data, test_print_string );
    tcase_add_test( tc_data, test_contains_string );

    /* error test cases */
    tcase_add_test( tc_error, error_test_create_destroy_null_attr_zero_capacity );
    tcase_add_test( tc_error, error_test_create_destroy_null_attr_negative_capacity );
    tcase_add_test( tc_error, error_test_create_destroy_attr_zero_capacity );
    tcase_add_test( tc_error, error_test_create_destroy_attr_negative_capacity );
    tcase_add_test( tc_error, error_test_insert_over_capacity );
    tcase_add_test( tc_error, error_test_insert_null_queue );
    tcase_add_test( tc_error, error_test_insert_null_item );
    tcase_add_test( tc_error, error_test_insert_null_queue_null_item );
    tcase_add_test( tc_error, error_test_remove_empty );
    tcase_add_test( tc_error, error_test_remove_null_queue );
    tcase_add_test( tc_error, error_test_clear_null_queue );
    tcase_add_test( tc_error, error_test_peek_null_queue );
    tcase_add_test( tc_error, error_test_count_null_param );
    tcase_add_test( tc_error, error_test_capacity_null_param );
    tcase_add_test( tc_error, error_test_reserve_negative_capacity );
    tcase_add_test( tc_error, error_test_reserve_zero_capacity );
    tcase_add_test( tc_error, error_test_reserve_capacity_too_small );
    tcase_add_test( tc_error, error_test_reserve_null_queue );
    tcase_add_test( tc_error, error_test_contains_null_queue );
    tcase_add_test( tc_error, error_test_contains_null_param );
    tcase_add_test( tc_error, error_test_remove_item_null_queue );
    tcase_add_test( tc_error, error_test_print_null_queue );
    tcase_add_test( tc_error, error_test_print_null_fp );
    
    /* add core tests to suite */
    suite_add_tcase( s, tc_core );
    suite_add_tcase( s, tc_data );
    suite_add_tcase( s, tc_error );

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
    //srunner_set_fork_status( sr, CK_NOFORK );
    srunner_run_all( sr, CK_VERBOSE );
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
