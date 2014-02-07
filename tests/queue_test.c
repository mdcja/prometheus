/**
 * Queue unit tests
 *
 * @brief This file provides queue unit tests
 * @author Julian Martinez del Campo
 * @license GNU Lesser General Public License (LGPL) https://www.gnu.org/copyleft/lesser.html
 *
 * This file provides unit tests for the queue ADT. Unit tests are run using the
 * Check framework available at http://check.sourceforge.net/.
 *
 * @note If the queue is implemented with a linked list the test_capacity and
 * test_reserve tests will fail.
 */

#include <stdlib.h>
#include <stdio.h>
#include <check.h>

#include "queue.h"
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
 * Test creation and deletion of a queue
 */
START_TEST( test_create_and_destroy )
{
    attr_t attributes = {0};
    queue_t * queue = NULL;
    int rc = 0;

    /* create queue without attributes */
    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );

    /* create a queue with default attributes */
    rc = attr_init( &attributes );
    ck_assert( rc == 0 );

    queue = queue_create( &attributes, DEFAULT_CAPACITY );
    ck_assert( rc == 0 );

    rc = queue_destroy( &queue );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test push/insert
 */
START_TEST( test_insert )
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
    error_code = 0; /* init error */
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
START_TEST( test_peek )
{
    queue_t * queue = NULL;
    int rc = -1;
    int number = 128;
    int * n = NULL;

    queue = queue_create( NULL, DEFAULT_CAPACITY );
    ck_assert( queue != NULL );

    /* test with empty queue */
    n = (int *)queue_peek( queue );
    ck_assert( n == NULL );

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

/**
 * Test count
 */
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
     * @see http://www.poetryfoundation.org/poem/173247
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
     * @see http://www.poetryfoundation.org/poem/173247
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

/**
 * Create a test suite
 */
Suite * queue_suite( void )
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
    Suite * s = queue_suite();
    SRunner * sr = srunner_create( s );
    //srunner_set_fork_status( sr, CK_NOFORK );
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
