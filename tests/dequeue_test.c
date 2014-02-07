/**
 * Dequeue unit tests
 *
 * @brief This file provides dequeue unit tests
 * @author Julian Martinez del Campo
 * @license GNU Lesser General Public License (LGPL) https://www.gnu.org/copyleft/lesser.html
 *
 * This file performs unit tests for the dequeue ADT. Unit tests are run using
 * the Check framework available at http://check.sourceforge.net/.
 *
 * @note If the dequeue is implemented with a linked list the test_capacity and
 * test_reserve tests will fail.
 */
#include <stdlib.h>
#include <stdio.h>
#include <check.h>

#include "dequeue.h"
#include "attributes.h"
#include "error.h"

#define DEFAULT_CAPACITY 100
#define SECONDARY_CAPACITY 200

#define MAX_STRLEN 256

/**
 * Compare two integers
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
 * Test creation and deletion of a dequeue
 */
START_TEST( test_create_and_destroy )
{
    attr_t attributes = {0};
    dequeue_t * dequeue = NULL;
    int rc = 0;

    /* initialize error code */
    error_code = 0;

    /* Create a dequeue without supplying attributes
     */
    dequeue = dequeue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( dequeue != NULL );

    rc = dequeue_destroy( &dequeue );
    ck_assert( rc == 0 );

    /* Create a dequeue with default attributes
     */
    rc = attr_init( &attributes );
    ck_assert( rc == 0 );

    dequeue = dequeue_create( &attributes, DEFAULT_CAPACITY );
    ck_assert( dequeue != NULL );

    rc = dequeue_destroy( &dequeue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test push/insert front
 */
START_TEST( test_insert_front )
{
    dequeue_t * dequeue = NULL;
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

    /* create dequeue */
    dequeue = dequeue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( dequeue != NULL );

    /* insert items */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = dequeue_insert_front( &dequeue, &numbers[ i ] );
        ck_assert( rc == 0 );

        /* check insertion */
        count = dequeue_count( dequeue );
        ck_assert( count == i + 1 );

        n = (int *)dequeue_peek_front( dequeue );
        ck_assert( n != NULL );
        ck_assert( *n == numbers[ i ] );
    }

    /* insert item into full dequeue */
    error_code = 0; /* init error */
    rc = dequeue_insert_front( &dequeue, &numbers[ 0 ] );

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

    dequeue_print( dequeue, stdout );

    /* destroy dequeue */
    rc = dequeue_destroy( &dequeue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test push/insert back
 */
START_TEST( test_insert_back )
{
    dequeue_t * dequeue = NULL;
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

    /* create dequeue */
    dequeue = dequeue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( dequeue != NULL );

    /* insert items */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = dequeue_insert_back( &dequeue, &numbers[ i ] );
        ck_assert( rc == 0 );

        /* check insertion */
        count = dequeue_count( dequeue );
        ck_assert( count == i + 1 );

        n = (int *)dequeue_peek_back( dequeue );
        ck_assert( n != NULL );
        ck_assert( *n == numbers[ i ] );
    }

    /* insert item into full dequeue */
    error_code = 0; /* init error */
    rc = dequeue_insert_back( &dequeue, &numbers[ 0 ] );


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

    /* destroy dequeue */
    rc = dequeue_destroy( &dequeue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test remove/pop front
 */
START_TEST( test_remove_front )
{
    dequeue_t * dequeue = NULL;
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
    dequeue = dequeue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( dequeue != NULL );

    /* remove from empty queue */
    error_code = 0;
    rc = dequeue_remove_front( &dequeue );
    ck_assert( rc == -1 );
    ck_assert( error_code == ERROR_UNDERFLOW );

    /* insert items */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = dequeue_insert_back( &dequeue, &numbers[ i ] );
        ck_assert( rc == 0 );
    }

    /* remove items */
    for( int i = DEFAULT_CAPACITY; i > 0; --i )
    {
        rc = dequeue_remove_front( &dequeue );
        ck_assert( rc == 0 );

        /* check removal */
        count = dequeue_count( dequeue );
        ck_assert( count == i - 1 );

        if( i > 1 )
        {
            n = (int *)dequeue_peek_front( dequeue );
            ck_assert( n != NULL );
            ck_assert( *n == numbers[ DEFAULT_CAPACITY - ( i - 1 )] );
        }
        else 
        {
            n = (int *)dequeue_peek_front( dequeue );
            ck_assert( n == NULL );
        }
    }

    /* destroy queue */
    rc = dequeue_destroy( &dequeue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test remove/pop back
 */
START_TEST( test_remove_back )
{
    dequeue_t * dequeue = NULL;
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

    /* create dequeue */
    dequeue = dequeue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( dequeue != NULL );

    /* remove from empty dequeue */
    error_code = 0;
    rc = dequeue_remove_back( &dequeue );
    ck_assert( rc == -1 );
    ck_assert( error_code == ERROR_UNDERFLOW );

    /* insert items */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = dequeue_insert_back( &dequeue, &numbers[ i ] );
        ck_assert( rc == 0 );
    }

    /* remove items */
    for( int i = DEFAULT_CAPACITY; i > 0; --i )
    {
        rc = dequeue_remove_back( &dequeue );
        ck_assert( rc == 0 );

        /* check removal */
        count = dequeue_count( dequeue );
        ck_assert( count == i - 1 );

        if( i > 1 )
        {
            n = (int *)dequeue_peek_back( dequeue );
            ck_assert( n != NULL );
            ck_assert( *n == numbers[ i - 2 ] );
        }
        else 
        {
            n = (int *)dequeue_peek_back( dequeue );
            ck_assert( n == NULL );
        }
    }

    /* destroy dequeue */
    rc = dequeue_destroy( &dequeue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test clear
 */
START_TEST( test_clear )
{
    dequeue_t * dequeue = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = {0};

    /* initialize test values
     */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    /* create dequeue */
    dequeue = dequeue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( dequeue != NULL );

    /* test clear on empty dequeue 
     */
    rc = dequeue_clear( &dequeue );
    ck_assert( rc == 0 );

    /* insert items */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = dequeue_insert_back( &dequeue, &numbers[ i ] );
        ck_assert( rc == 0 );
    }

    /* test clear on full list
     */
    rc = dequeue_clear( &dequeue );
    ck_assert( rc == 0 );
    ck_assert( dequeue_count( dequeue ) == 0 );

    /* destroy dequeue */
    rc = dequeue_destroy( &dequeue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test memory usage
 */
START_TEST( test_memory )
{
    dequeue_t * dequeue = NULL;
    int rc = -1;
    int memory_usage = -1;

    dequeue = dequeue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( dequeue != NULL );

    /* test memory usage */
    memory_usage = dequeue_memory( dequeue );
    ck_assert( memory_usage > 0 );

    rc = dequeue_destroy( &dequeue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test capacity
 */
START_TEST( test_capacity )
{
    dequeue_t * dequeue = NULL;
    int rc = -1;
    int capacity = -1;

    error_code = 0;

    dequeue = dequeue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( dequeue != NULL );

    /* get capacity */
    capacity = dequeue_capacity( dequeue );
    ck_assert( capacity == DEFAULT_CAPACITY );

    rc = dequeue_destroy( &dequeue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test peek front
 */
START_TEST( test_peek_front )
{
    dequeue_t * dequeue = NULL;
    int rc = -1;
    int number = 128;
    int * n = NULL;

    dequeue = dequeue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( dequeue != NULL );

    /* test with empty dequeue */
    n = (int *)dequeue_peek_front( dequeue );
    ck_assert( n == NULL );

    /* test with item in dequeue */
    rc = dequeue_insert_back( &dequeue, &number );
    ck_assert( rc == 0 );

    n = (int *)dequeue_peek_front( dequeue );
    ck_assert( n != NULL );
    ck_assert( *n == number );

    rc = dequeue_destroy( &dequeue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test peek back
 */
START_TEST( test_peek_back )
{
    dequeue_t * dequeue = NULL;
    int rc = -1;
    int number = 128;
    int * n = NULL;

    dequeue = dequeue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( dequeue != NULL );

    /* test with empty dequeue */
    n = (int *)dequeue_peek_back( dequeue );
    ck_assert( n == NULL );

    /* test with item in dequeue */
    rc = dequeue_insert_back( &dequeue, &number );
    ck_assert( rc == 0 );

    n = (int *)dequeue_peek_back( dequeue );
    ck_assert( n != NULL );
    ck_assert( *n == number );

    rc = dequeue_destroy( &dequeue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test count
 */
START_TEST( test_count )
{
    dequeue_t * dequeue = NULL;
    int rc = -1;
    int count = -1;
    int numbers[ DEFAULT_CAPACITY ] = {0};

    /* initialize test values
     */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    /* create dequeue */
    dequeue = dequeue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( dequeue != NULL );

    /* get count on empty dequeue */
    count = dequeue_count( dequeue );
    ck_assert( count == 0 );

    /* insert items and check count */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = dequeue_push_front( &dequeue, &numbers[ i ] );
        ck_assert( rc == 0 );

        count = dequeue_count( dequeue );
        ck_assert( count == i + 1 );
    }

    /* remove items and check count */
    for( int i = DEFAULT_CAPACITY; i > 0; --i )
    {
        rc = dequeue_pop_front( &dequeue );
        ck_assert( rc == 0 );

        count = dequeue_count( dequeue );
        ck_assert( count = DEFAULT_CAPACITY - 1 );
    }

    rc = dequeue_destroy( &dequeue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test reserve
 */
START_TEST( test_reserve )
{
    dequeue_t * dequeue = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = {0};

    /* initialize test values */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    dequeue = dequeue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( dequeue != NULL );
    
    /* resize an empty dequeue */
    ck_assert( dequeue_capacity( dequeue ) == DEFAULT_CAPACITY );

    rc = dequeue_reserve( &dequeue, SECONDARY_CAPACITY );
    ck_assert( rc == 0 );
    ck_assert( dequeue_capacity( dequeue ) == SECONDARY_CAPACITY );

    /* resize back to default capacity */
    rc = dequeue_reserve( &dequeue, DEFAULT_CAPACITY );
    ck_assert( rc == 0 );
    ck_assert( dequeue_capacity( dequeue ) == DEFAULT_CAPACITY );

    /* add items to dequeue and resize again */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = dequeue_push_front( &dequeue, &numbers[ i ] );
        ck_assert( rc == 0 );
    }

    rc = dequeue_reserve( &dequeue, SECONDARY_CAPACITY );
    ck_assert( rc == 0 );
    ck_assert( dequeue_capacity( dequeue ) ==  SECONDARY_CAPACITY );

    rc = dequeue_destroy( &dequeue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test contains
 */
START_TEST( test_contains )
{
    dequeue_t * dequeue = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = {0};
    int z = -5;

    /* initialize test values */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    dequeue = dequeue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( dequeue != NULL );

    /* test emtpy dequeue */
    rc = dequeue_contains( dequeue, &z );
    ck_assert( rc == 0 );

    /* add data to dequeue */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = dequeue_push_front( &dequeue, &numbers[ i ] );
        ck_assert( rc == 0 );
    }

    /* look for data NOT in dequeue */
    rc = dequeue_contains( dequeue, &z );
    ck_assert( rc == 0 );

    /* look for data in dequeue */
    rc = dequeue_contains( dequeue, &numbers[0] );
    ck_assert( rc == 1 );

    rc = dequeue_destroy( &dequeue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test remove item
 */
START_TEST( test_remove_item )
{
    dequeue_t * dequeue = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = {0};
    int z = -5;

    /* initialize test values */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    dequeue = dequeue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( dequeue != NULL );

    /* remove item from empty dequeue */
    error_code = 0;
    rc = dequeue_remove_item( &dequeue, &z );
    ck_assert( rc == -1 );
    ck_assert( error_code == ERROR_NOT_FOUND );

    /* add data to dequeue */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = dequeue_push_front( &dequeue, &numbers[ i ] );
        ck_assert( rc == 0 );
    }

    /* remove nonexistent item from dequeue */
    error_code = 0;
    rc = dequeue_remove_item( &dequeue, &z );
    ck_assert( rc == -1 );
    ck_assert( error_code == ERROR_NOT_FOUND );

    /* remove item from dequeue */
    rc = dequeue_remove_item( &dequeue, &numbers[ 0 ] );
    ck_assert( rc == 0 );
    ck_assert( dequeue_count( dequeue ) == DEFAULT_CAPACITY - 1 );

    rc = dequeue_destroy( &dequeue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test print
 */
START_TEST( test_print )
{
    dequeue_t * dequeue = NULL;
    int rc = 0;
    int nchr = -1;
    int numbers[ DEFAULT_CAPACITY ] = {0};

    /* initialize test values
     */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    dequeue = dequeue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( dequeue != NULL );

    /* print empty dequeue */
    nchr = dequeue_print( dequeue, stdout );
    ck_assert( nchr > 0 );

    /* insert items and print dequeue */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = dequeue_insert_back( &dequeue, &numbers[ i ] );
        ck_assert( rc == 0 );
    }

    nchr = dequeue_print( dequeue, stdout );
    ck_assert( nchr > 0 );

    rc = dequeue_destroy( &dequeue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test print integer
 */
START_TEST( test_print_int )
{
    dequeue_t * dequeue = NULL;
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

    dequeue = dequeue_create( &attributes, DEFAULT_CAPACITY );
    ck_assert( dequeue != NULL );

    /* print empty dequeue */
    nchr = dequeue_print( dequeue, stdout );
    ck_assert( nchr > 0 );

    /* insert items and print dequeue */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = dequeue_insert_back( &dequeue, &numbers[ i ] );
        ck_assert( rc == 0 );
    }

    nchr = dequeue_print( dequeue, stdout );
    ck_assert( nchr > 0 );

    rc = dequeue_destroy( &dequeue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test string print
 */
START_TEST( test_print_string )
{
    dequeue_t * dequeue = NULL;
    attr_t attributes = {0};
    int rc = -1;
    int nchr = -1;

    /**
     * The first 101 words of the poem "The Dream" by Lord Byron
     */
    char * words[ MAX_STRLEN ] = { 
        "OUR", "life", "is", "twofold;", "sleep", "hath", "its", "own",
        "world,", "A", "boundary", "between", "the", "things", "misnamed",
        "Death", "and", "existence:", "sleep", "hath", "its", "own", "world,",
        "And", "a", "wide", "realm", "of", "wild", "reality,", "And", "dreams",
        "in", "their", "development", "have", "breath,", "And", "tears,",
        "and", "tortures,", "and", "the", "touch", "of", "joy;", "They",
        "leave", "a", "weight", "upon", "our", "waking", "thoughts,", "They",
        "take", "a", "weight", "from", "off", "our", "waking", "toils,",
        "They", "do", "divide", "our", "being;", "they", "become", "A",
        "portion", "of", "ourselves", "as", "of", "our", "time,", "And",
        "look", "like", "heralds", "of", "eternity;", "They", "pass", "like",
        "spirits", "of", "the", "past,—they", "speak", "Like", "sibyls", "of",
        "the", "future;", "they", "have", "power,—"
    };

    /* set integer data */
    attr_init( &attributes );
    attr_set_compare( &attributes, string_compare );
    attr_set_print( &attributes, string_print );
    attr_set_copy( &attributes, string_copy );
    attr_set_free( &attributes, string_free );

    dequeue = dequeue_create( &attributes, DEFAULT_CAPACITY );
    ck_assert( dequeue != NULL );

    /* print empty dequeue */
    nchr = dequeue_print( dequeue, stdout );
    ck_assert( nchr > 0 );

    /* insert items and print dequeue */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = dequeue_insert_back( &dequeue, words[ i ] );
        ck_assert( rc == 0 );
    }

    nchr = dequeue_print( dequeue, stdout );
    ck_assert( nchr > 0 );

    rc = dequeue_destroy( &dequeue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test contains with a string
 */
START_TEST( test_contains_string )
{
    dequeue_t * dequeue = NULL;
    attr_t attributes = {0};
    int rc = -1;
    /**
     * The first 101 words of the poem "The Dream" by Lord Byron
     */
    char * words[ MAX_STRLEN ] = { 
        "OUR", "life", "is", "twofold;", "sleep", "hath", "its", "own",
        "world,", "A", "boundary", "between", "the", "things", "misnamed",
        "Death", "and", "existence:", "sleep", "hath", "its", "own", "world,",
        "And", "a", "wide", "realm", "of", "wild", "reality,", "And", "dreams",
        "in", "their", "development", "have", "breath,", "And", "tears,",
        "and", "tortures,", "and", "the", "touch", "of", "joy;", "They",
        "leave", "a", "weight", "upon", "our", "waking", "thoughts,", "They",
        "take", "a", "weight", "from", "off", "our", "waking", "toils,",
        "They", "do", "divide", "our", "being;", "they", "become", "A",
        "portion", "of", "ourselves", "as", "of", "our", "time,", "And",
        "look", "like", "heralds", "of", "eternity;", "They", "pass", "like",
        "spirits", "of", "the", "past,—they", "speak", "Like", "sibyls", "of",
        "the", "future;", "they", "have", "power,—"
    };

    /* set integer data */
    attr_init( &attributes );
    attr_set_compare( &attributes, string_compare );
    attr_set_print( &attributes, string_print );
    attr_set_copy( &attributes, string_copy );
    attr_set_free( &attributes, string_free );

    dequeue = dequeue_create( &attributes, DEFAULT_CAPACITY );
    ck_assert( dequeue != NULL );

    /* insert items and print dequeue */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = dequeue_insert_back( &dequeue, words[ i ] );
        ck_assert( rc == 0 );
    }

    /* check for words */
    rc = dequeue_contains( dequeue, "divide" );
    ck_assert( rc == 1 );

    rc = dequeue_contains( dequeue, "existence:" );
    ck_assert( rc == 1 );

    /* not in dequeue */
    rc = dequeue_contains( dequeue, "foo" );
    ck_assert( rc == 0 );

    rc = dequeue_destroy( &dequeue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Create a test suite
 */
Suite * dequeue_suite( void )
{
    Suite * s = suite_create( "Dequeue" );
    TCase * tc_core = tcase_create( "Core" );
    TCase * tc_data = tcase_create( "Data" );
    TCase * tc_error = tcase_create( "Error" );

    /* core test cases */
    tcase_add_test( tc_core, test_create_and_destroy );
    tcase_add_test( tc_core, test_insert_front );
    tcase_add_test( tc_core, test_insert_back );
    tcase_add_test( tc_core, test_remove_front );
    tcase_add_test( tc_core, test_remove_back );
    tcase_add_test( tc_core, test_clear );
    tcase_add_test( tc_core, test_memory );
    tcase_add_test( tc_core, test_capacity );
    tcase_add_test( tc_core, test_peek_front );
    tcase_add_test( tc_core, test_peek_back );
    tcase_add_test( tc_core, test_count );
    tcase_add_test( tc_core, test_reserve );
    tcase_add_test( tc_core, test_contains );
    tcase_add_test( tc_core, test_remove_item );
    tcase_add_test( tc_core, test_print );

    /* data tests */
    tcase_add_test( tc_core, test_print_int );
    tcase_add_test( tc_core, test_print_string );
    tcase_add_test( tc_core, test_contains_string );

    /* error test cases */
    
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
    Suite * s = dequeue_suite();
    SRunner * sr = srunner_create( s );

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
