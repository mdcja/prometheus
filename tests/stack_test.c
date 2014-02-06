/**
 * Stack unit tests
 *
 * @brief This file provides stack unit tests
 * @author Julian Martinez del Campo
 * @license GNU Lesser General Public License (LGPL) https://www.gnu.org/copyleft/lesser.html
 *
 * This file provides unit tests for the stack ADT. Unit tests are run using the
 * Check framework available at http://check.sourceforge.net/.
 *
 * Due to the abstract nature of the stack, concrete unit tests are difficult
 * to perform. The tests run on the stack try to address this problem by
 * testing the output and behavior of the data contained in the stack. 
 *
 * @note If the stack is implemented with a linked list the test_capacity and
 * test_reserve tests will fail.
 */

#include <stdlib.h>
#include <stdio.h>
#include <check.h>

#include "stack.h"
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
 * Test creation and deletion of a stack
 */
START_TEST( test_create_and_destroy )
{
    attr_t attributes = {0};
    stack_t * stack = NULL;
    int rc = 0;

    /* initialize error code */
    error_code = 0;

    /* Create a stack without supplying attributes
     */
    stack = stack_create( NULL, DEFAULT_CAPACITY );
    ck_assert( stack != NULL );

    rc = stack_destroy( &stack );
    ck_assert( rc == 0 );

    /* Create a stack with default attributes
     */
    rc = attr_init( &attributes );
    ck_assert( rc == 0 );

    stack = stack_create( &attributes, DEFAULT_CAPACITY );
    ck_assert( stack != NULL );

    rc = stack_destroy( &stack );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test push/insert
 */
START_TEST( test_insert )
{
    stack_t * stack = NULL;
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

    /* create stack */
    stack = stack_create( NULL, DEFAULT_CAPACITY );
    ck_assert( stack != NULL );

    /* insert items */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = stack_insert( &stack, &numbers[ i ] );
        ck_assert( rc == 0 );

        /* check insertion */
        count = stack_count( stack );
        ck_assert( count == i + 1 );

        n = (int *)stack_peek( stack );
        ck_assert( n != NULL );
        ck_assert( *n == numbers[ i ] );
    }

    /* insert item into full stack */
    error_code = 0; /* init error */
    rc = stack_insert( &stack, &numbers[ 0 ] );

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

    /* destroy stack */
    rc = stack_destroy( &stack );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test remove/pop
 */
START_TEST( test_remove )
{
    stack_t * stack = NULL;
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

    /* create stack */
    stack = stack_create( NULL, DEFAULT_CAPACITY );
    ck_assert( stack != NULL );

    /* remove from empty stack */
    error_code = 0;
    rc = stack_remove( &stack );
    ck_assert( rc == -1 );
    ck_assert( error_code == ERROR_UNDERFLOW );

    /* insert items */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = stack_insert( &stack, &numbers[ i ] );
        ck_assert( rc == 0 );
    }

    /* remove items */
    for( int i = DEFAULT_CAPACITY; i > 0; --i )
    {
        rc = stack_remove( &stack );
        ck_assert( rc == 0 );

        /* check removal */
        count = stack_count( stack );
        ck_assert( count == i - 1 );

        if( i > 1 )
        {
            n = (int *)stack_peek( stack );
            ck_assert( n != NULL );
            ck_assert( *n == numbers[ i - 2 ] );
        }
        else 
        {
            n = (int *)stack_peek( stack );
            ck_assert( n == NULL );
        }
    }

    /* destroy stack */
    rc = stack_destroy( &stack );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test clear
 */
START_TEST( test_clear )
{
    stack_t * stack = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = {0};

    /* initialize test values
     */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    /* create stack */
    stack = stack_create( NULL, DEFAULT_CAPACITY );
    ck_assert( stack != NULL );

    /* test clear on empty stack 
     */
    rc = stack_clear( &stack );
    ck_assert( rc == 0 );

    /* insert items */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = stack_insert( &stack, &numbers[ i ] );
        ck_assert( rc == 0 );
    }

    /* test clear on full list
     */
    rc = stack_clear( &stack );
    ck_assert( rc == 0 );
    ck_assert( stack_count( stack ) == 0 );

    /* destroy stack */
    rc = stack_destroy( &stack );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test memory usage
 */
START_TEST( test_memory )
{
    stack_t * stack = NULL;
    int rc = -1;
    int memory_usage = -1;

    stack = stack_create( NULL, DEFAULT_CAPACITY );
    ck_assert( stack != NULL );

    /* test memory usage */
    memory_usage = stack_memory( stack );
    ck_assert( memory_usage > 0 );

    rc = stack_destroy( &stack );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test capacity
 */
START_TEST( test_capacity )
{
    stack_t * stack = NULL;
    int rc = -1;
    int capacity = -1;

    error_code = 0;

    stack = stack_create( NULL, DEFAULT_CAPACITY );
    ck_assert( stack != NULL );

    /* get capacity */
    capacity = stack_capacity( stack );
    ck_assert( capacity == DEFAULT_CAPACITY );

    rc = stack_destroy( &stack );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test peek
 */
START_TEST( test_peek )
{
    stack_t * stack = NULL;
    int rc = -1;
    int number = 128;
    int * n = NULL;

    stack = stack_create( NULL, DEFAULT_CAPACITY );
    ck_assert( stack != NULL );

    /* test with empty stack */
    n = (int *)stack_peek( stack );
    ck_assert( n == NULL );

    /* test with item in stack */
    rc = stack_insert( &stack, &number );
    ck_assert( rc == 0 );

    n = (int *)stack_peek( stack );
    ck_assert( n != NULL );
    ck_assert( *n == number );

    rc = stack_destroy( &stack );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test count
 */
START_TEST( test_count )
{
    stack_t * stack = NULL;
    int rc = -1;
    int count = -1;
    int numbers[ DEFAULT_CAPACITY ] = {0};

    /* initialize test values
     */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    /* create stack */
    stack = stack_create( NULL, DEFAULT_CAPACITY );
    ck_assert( stack != NULL );

    /* get count on empty stack */
    count = stack_count( stack );
    ck_assert( count == 0 );

    /* insert items and check count */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = stack_push( &stack, &numbers[ i ] );
        ck_assert( rc == 0 );

        count = stack_count( stack );
        ck_assert( count == i + 1 );
    }

    /* remove items and check count */
    for( int i = DEFAULT_CAPACITY; i > 0; --i )
    {
        rc = stack_pop( &stack );
        ck_assert( rc == 0 );

        count = stack_count( stack );
        ck_assert( count = DEFAULT_CAPACITY - 1 );
    }

    rc = stack_destroy( &stack );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test reserve
 */
START_TEST( test_reserve )
{
    stack_t * stack = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = {0};

    /* initialize test values */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    stack = stack_create( NULL, DEFAULT_CAPACITY );
    ck_assert( stack != NULL );
    
    /* resize an empty stack */
    ck_assert( stack_capacity( stack ) == DEFAULT_CAPACITY );

    rc = stack_reserve( &stack, SECONDARY_CAPACITY );
    ck_assert( rc == 0 );
    ck_assert( stack_capacity( stack ) == SECONDARY_CAPACITY );

    /* resize back to default capacity */
    rc = stack_reserve( &stack, DEFAULT_CAPACITY );
    ck_assert( rc == 0 );
    ck_assert( stack_capacity( stack ) == DEFAULT_CAPACITY );

    /* add items to stack and resize again */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = stack_push( &stack, &numbers[ i ] );
        ck_assert( rc == 0 );
    }

    rc = stack_reserve( &stack, SECONDARY_CAPACITY );
    ck_assert( rc == 0 );
    ck_assert( stack_capacity( stack ) ==  SECONDARY_CAPACITY );

    rc = stack_destroy( &stack );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test contains
 */
START_TEST( test_contains )
{
    stack_t * stack = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = {0};
    int z = -5;

    /* initialize test values */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    stack = stack_create( NULL, DEFAULT_CAPACITY );
    ck_assert( stack != NULL );

    /* test emtpy stack */
    rc = stack_contains( stack, &z );
    ck_assert( rc == 0 );

    /* add data to stack */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = stack_push( &stack, &numbers[ i ] );
        ck_assert( rc == 0 );
    }

    /* look for data NOT in stack */
    rc = stack_contains( stack, &z );
    ck_assert( rc == 0 );

    /* look for data in stack */
    rc = stack_contains( stack, &numbers[0] );
    ck_assert( rc == 1 );

    rc = stack_destroy( &stack );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test remove item
 */
START_TEST( test_remove_item )
{
    stack_t * stack = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = {0};
    int z = -5;

    /* initialize test values */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    stack = stack_create( NULL, DEFAULT_CAPACITY );
    ck_assert( stack != NULL );

    /* remove item from empty stack */
    error_code = 0;
    rc = stack_remove_item( &stack, &z );
    ck_assert( rc == -1 );
    ck_assert( error_code == ERROR_NOT_FOUND );

    /* add data to stack */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = stack_push( &stack, &numbers[ i ] );
        ck_assert( rc == 0 );
    }

    /* remove nonexistent item from stack */
    error_code = 0;
    rc = stack_remove_item( &stack, &z );
    ck_assert( rc == -1 );
    ck_assert( error_code == ERROR_NOT_FOUND );

    /* remove item from stack */
    rc = stack_remove_item( &stack, &numbers[ 0 ] );
    ck_assert( rc == 0 );
    ck_assert( stack_count( stack ) == DEFAULT_CAPACITY - 1 );

    rc = stack_destroy( &stack );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test print
 */
START_TEST( test_print )
{
    stack_t * stack = NULL;
    int rc = 0;
    int nchr = -1;
    int numbers[ DEFAULT_CAPACITY ] = {0};

    /* initialize test values
     */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    stack = stack_create( NULL, DEFAULT_CAPACITY );
    ck_assert( stack != NULL );

    /* print empty stack */
    nchr = stack_print( stack, stdout );
    ck_assert( nchr > 0 );

    /* insert items and print stack */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = stack_insert( &stack, &numbers[ i ] );
        ck_assert( rc == 0 );
    }

    nchr = stack_print( stack, stdout );
    ck_assert( nchr > 0 );

    rc = stack_destroy( &stack );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test print integer
 */
START_TEST( test_print_int )
{
    stack_t * stack = NULL;
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

    stack = stack_create( &attributes, DEFAULT_CAPACITY );
    ck_assert( stack != NULL );

    /* print empty stack */
    nchr = stack_print( stack, stdout );
    ck_assert( nchr > 0 );

    /* insert items and print stack */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = stack_insert( &stack, &numbers[ i ] );
        ck_assert( rc == 0 );
    }

    nchr = stack_print( stack, stdout );
    ck_assert( nchr > 0 );

    rc = stack_destroy( &stack );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test string print
 */
START_TEST( test_print_string )
{
    stack_t * stack = NULL;
    attr_t attributes = {0};
    int rc = -1;
    int nchr = -1;
    /** 
     * The first 100 words of the poem "Howl" by Alan Ginsberg 
     */
    char * words[ MAX_STRLEN ] = { "I", "saw", "the",  "best",  "minds",
    "of",  "my",  "generation",  "destroyed",  "by",  "madness,",  "starving",
    "hysterical",  "naked,",  "dragging",  "themselves",  "through",  "the",
    "negro",  "streets",  "at",  "dawn",  "looking",  "for",  "an",  "angry",
    "fix,",  "angelheaded",  "hipsters",  "burning",  "for",  "the",
    "ancient",  "heavenly",  "connection",  "to",  "the",  "starry",  "dynamo",
    "in",  "the",  "machinery",  "of",  "night,",  "who",  "poverty",  "and",
    "tatters",  "and",  "hollow-eyed",  "and",  "high",  "sat",  "up",
    "smoking",  "in",  "the",  "supernatural",  "darkness",  "of",
    "cold-water",  "flats",  "floating",  "across",  "the",  "tops",  "of",
    "cities",  "contemplating",  "jazz,",  "who",  "bared",  "their",
    "brains",  "to",  "Heaven",  "under",  "the",  "El",  "and",  "saw",
    "Mohammedan",  "angels",  "staggering",  "on",  "tenement",  "roofs",
    "illuminated,",  "who",  "passed",  "through",  "universities",  "with",
    "radiant",  "cool",  "eyes",  "hallucinating",  "Arkan-sas",  "and", "..."
    };

    /* set integer data */
    attr_init( &attributes );
    attr_set_compare( &attributes, string_compare );
    attr_set_print( &attributes, string_print );
    attr_set_copy( &attributes, string_copy );
    attr_set_free( &attributes, string_free );

    stack = stack_create( &attributes, DEFAULT_CAPACITY );
    ck_assert( stack != NULL );

    /* print empty stack */
    nchr = stack_print( stack, stdout );
    ck_assert( nchr > 0 );

    /* insert items and print stack */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = stack_insert( &stack, words[ i ] );
        ck_assert( rc == 0 );
    }

    nchr = stack_print( stack, stdout );
    ck_assert( nchr > 0 );

    rc = stack_destroy( &stack );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test contains with a string
 */
START_TEST( test_contains_string )
{
    stack_t * stack = NULL;
    attr_t attributes = {0};
    int rc = -1;
    /** 
     * The first 100 words of the poem "Howl" by Alan Ginsberg 
     */
    char * words[ MAX_STRLEN ] = { "I", "saw", "the",  "best",  "minds",
    "of",  "my",  "generation",  "destroyed",  "by",  "madness,",  "starving",
    "hysterical",  "naked,",  "dragging",  "themselves",  "through",  "the",
    "negro",  "streets",  "at",  "dawn",  "looking",  "for",  "an",  "angry",
    "fix,",  "angelheaded",  "hipsters",  "burning",  "for",  "the",
    "ancient",  "heavenly",  "connection",  "to",  "the",  "starry",  "dynamo",
    "in",  "the",  "machinery",  "of",  "night,",  "who",  "poverty",  "and",
    "tatters",  "and",  "hollow-eyed",  "and",  "high",  "sat",  "up",
    "smoking",  "in",  "the",  "supernatural",  "darkness",  "of",
    "cold-water",  "flats",  "floating",  "across",  "the",  "tops",  "of",
    "cities",  "contemplating",  "jazz,",  "who",  "bared",  "their",
    "brains",  "to",  "Heaven",  "under",  "the",  "El",  "and",  "saw",
    "Mohammedan",  "angels",  "staggering",  "on",  "tenement",  "roofs",
    "illuminated,",  "who",  "passed",  "through",  "universities",  "with",
    "radiant",  "cool",  "eyes",  "hallucinating",  "Arkan-sas",  "and", "..."
    };

    /* set integer data */
    attr_init( &attributes );
    attr_set_compare( &attributes, string_compare );
    attr_set_print( &attributes, string_print );
    attr_set_copy( &attributes, string_copy );
    attr_set_free( &attributes, string_free );

    stack = stack_create( &attributes, DEFAULT_CAPACITY );
    ck_assert( stack != NULL );

    /* insert items and print stack */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = stack_insert( &stack, words[ i ] );
        ck_assert( rc == 0 );
    }

    /* check for words */
    rc = stack_contains( stack, "generation" );
    ck_assert( rc == 1 );

    rc = stack_contains( stack, "illuminated," );
    ck_assert( rc == 1 );

    /* not in stack */
    rc = stack_contains( stack, "foo" );
    ck_assert( rc == 0 );

    rc = stack_destroy( &stack );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Create a test suite
 */
Suite * stack_suite( void )
{
    Suite * s = suite_create( "Stack" );
    TCase * tc_core = tcase_create( "Core" );
    TCase * tc_data = tcase_create( "Data" );
    TCase * tc_error = tcase_create( "Error" );

    /* core test cases */
    tcase_add_test( tc_core, test_create_and_destroy );
    tcase_add_test( tc_core, test_insert );
    tcase_add_test( tc_core, test_remove );
    tcase_add_test( tc_core, test_clear );
    tcase_add_test( tc_core, test_memory );
    tcase_add_test( tc_core, test_capacity );
    tcase_add_test( tc_core, test_peek );
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
    Suite * s = stack_suite();
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
