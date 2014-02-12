/**
 * This file provides a hash table implemented using open addressing
 *
 * @brief Provides a hash table ADT implemented using open addressing.
 * @author Julian Martinez del Campo
 * @license GNU Lesser General Public License (LGPL) https://www.gnu.org/copyleft/lesser.html
 *
 * This file provides an implementation of a hash table using open addressing.
 * Collisions are resolved via linear probing.
 *
 * The memory complexity of this hash table is O(1).
 *
 * This implementation provides the following run time complexity for a hash
 * table with n elements.
 *
 * Operation    | Complexity
 *              |-------------
 *              | Best | Worst
 * -------------|------|------
 * create       | O(1) |
 * destroy      | O(n) |
 * clear        | O(n) |
 * reserve      | O(n) | O(n^2)
 * insert       | O(1) | O(n)
 * lookup       | O(1) | O(n)
 * remove       | O(1) | O(n)
 * print        | O(n) |
 * load         | O(1) |
 * capacity     | O(1) |
 * count        | O(1) |
 * memory       | O(1) |
 *
 * This implementation has an average run time complexity for successful
 * searches: 1 + (a/2), where a is the load on the table.
 *
 * This implementation has an average run time complexity for unsucessful
 * searches is: a, where a is the load on the table.
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "hash_table.h"
#include "error.h"
#include "attributes.h"

/**
 * Hash table node
 */
struct _hash_table_node {
    void * key;
    void * data;
};

/**
 * A hash table
 */
struct _hash_table_type {
    struct _hash_table_node ** hash_table;
    int * marker;
    int capacity;
    int count;
    int memory;
    attr_t kattr;
    attr_t dattr;
};

/**
 * Create a new hash table.
 */
hash_table_t * hash_table_create( const attr_t * kattr, const attr_t * dattr, int capacity )
{
    hash_table_t * new_hash_table = NULL;

    /* check */
    if( capacity < 1 )
    {
        error_code = ERROR_INVALID_CAPACITY;
        error_print( "hash_table_create" );

        return NULL;
    }

    /* create */
    new_hash_table = (hash_table_t *)calloc( 1, sizeof( hash_table_t ) );
    assert( new_hash_table );
    if( !new_hash_table )
    {
        error_code = ERROR_MEMORY_ALLOCATION_FAILED;
        error_print( "hash_table_create" );

        return NULL;
    }

    new_hash_table->hash_table = (struct _hash_table_node **)calloc( capacity, sizeof( struct _hash_table_node ) );
    assert( new_hash_table->hash_table );
    if( !new_hash_table->hash_table )
    {
        error_code = ERROR_MEMORY_ALLOCATION_FAILED;
        error_print( "hash_table_create" );

        return NULL;
    }

    new_hash_table->marker = (int *)calloc( capacity, sizeof( int ) );
    assert( new_hash_table->marker );
    if( !new_hash_table->marker )
    {
        error_code = ERROR_MEMORY_ALLOCATION_FAILED;
        error_print( "hash_table_create" );

        return NULL;
    }

    /* init */
    new_hash_table->capacity = capacity;
    new_hash_table->count = 0;
    new_hash_table->memory = sizeof( hash_table_t ) + capacity * sizeof( struct _hash_table_node * );

    if( !kattr )
    {
        attr_init( &new_hash_table->kattr );
    }
    else
    {
        new_hash_table->kattr = *kattr;
    }

    if( !dattr )
    {
        attr_init( &new_hash_table->dattr );
    }
    else
    {
        new_hash_table->dattr = *dattr;
    }

    assert( new_hash_table->kattr.copy );
    assert( new_hash_table->kattr.free );
    assert( new_hash_table->kattr.compare );
    assert( new_hash_table->kattr.print );
    assert( new_hash_table->kattr.hash );
    assert( new_hash_table->dattr.copy );
    assert( new_hash_table->dattr.free );
    assert( new_hash_table->dattr.print );
    
    return new_hash_table;
}

/**
 * Destroy a hash table.
 */
int hash_table_destroy( hash_table_t ** ht )
{
    /* check */
    if( !ht )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "hash_table_destroy" );

        return -1;
    }

    if( !*ht )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "hash_table_destroy" );

        return -1;
    }

    /* deallocate all items in the hash table */
    if( hash_table_clear( ht ) != 0 )
    {
        return -1;
    }

    /* deallocate */
    free( (*ht)->hash_table );
    (*ht)->hash_table = NULL;

    free( *ht );
    *ht = NULL;

    return 0;
}

/**
 * Clear a hash table.
 */
int hash_table_clear( hash_table_t ** ht )
{
    struct _hash_table_node * temp = NULL;
    int i = 0;

    /* check */
    if( !ht )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "hash_table_clear" );

        return -1;
    }

    if( !*ht )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "hash_table_clear" );

        return -1;
    }

    if( (*ht)->count == 0 )
    {
        return 0;
    }

    assert( (*ht)->kattr.free );
    assert( (*ht)->dattr.free );

    for( i = 0; i < (*ht)->capacity; ++i )
    {
        temp = (*ht)->hash_table[ i ];
        if( temp )
        {
            /* free */
            (*ht)->kattr.free( temp->key );
            temp->key = NULL;

            (*ht)->dattr.free( temp->data );
            temp->data = NULL;

            free( temp );
            temp = NULL;

            (*ht)->count--;
        }
        (*ht)->marker[ i ] = 0;
    }

    assert( (*ht)->count == 0 );

    return 0;
}

/**
 * Resize a hash table.
 */
int hash_table_reserve( hash_table_t ** ht, int capacity )
{
    struct _hash_table_node ** new_hash_table = NULL;
    struct _hash_table_node ** old_hash_table = NULL;
    int * new_marker = NULL;
    int * old_marker = NULL;
    int idx = 0;
    int i = 0;
    int j = 0;

    /* check */
    if( !ht )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "hash_table_reserve" );

        return -1;
    }

    if( !*ht )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "hash_table_reserve" );

        return -1;
    }

    if( capacity < (*ht)->count )
    {
        error_code = ERROR_INVALID_CAPACITY;
        error_print( "hash_table_reserve" );

        return -1;
    }

    assert( (*ht)->kattr.hash );

    /* create */
    new_hash_table = (struct _hash_table_node **)calloc( capacity, sizeof( struct _hash_table_node * ) );
    assert( new_hash_table );
    if( !new_hash_table )
    {
        error_code = ERROR_MEMORY_ALLOCATION_FAILED;
        error_print( "hash_table_reserve" );

        return -1;
    }

    new_marker = (int *)calloc( capacity, sizeof( int ) );
    assert( new_marker );
    if( !new_marker )
    {
        error_code = ERROR_MEMORY_ALLOCATION_FAILED;
        error_print( "hash_table_reserve" );

        return -1;
    }

    /* copy */
    for( i = 0; i < (*ht)->capacity; ++i )
    {
        if( (*ht)->hash_table[ i ] )
        {
            /* re-insert */
            idx = (*ht)->kattr.hash( (*ht)->hash_table[ i ]->key ) % capacity;
            if( new_hash_table[ idx ] )
            {
                for( j = 1; j < capacity; ++j )
                {
                    if( !new_hash_table[ (idx + j) % capacity ] )
                    {
                        new_hash_table[ (idx + j) % capacity ] = (*ht)->hash_table[ i ];
                    }
                }
            }
            else
            {
                new_hash_table[ idx ] = (*ht)->hash_table[ i ];
            }
        }
    }

    /* replace hash table */
    old_hash_table = (*ht)->hash_table;
    old_marker = (*ht)->marker;
    (*ht)->hash_table = new_hash_table;
    (*ht)->marker = new_marker;
    (*ht)->capacity = capacity;

    /* deallocate */
    free( old_hash_table );
    old_hash_table = NULL;

    free( old_marker );
    old_marker = NULL;

    return 0;
}

/**
 * Insert/Update item into a hash table.
 */
int hash_table_insert( hash_table_t ** ht, void * key, void * data )
{
    struct _hash_table_node * new_node = NULL;
    int idx = 0;
    int i = 0;

    /* check */
    if( !ht )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "hash_table_insert" );

        return -1;
    }

    if( !*ht )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "hash_table_insert" );

        return -1;
    }

    if( !key )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "hash_table_insert" );

        return -1;
    }

    if( !data )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "hash_table_insert" );

        return -1;
    }

    if( (*ht)->count >= (*ht)->capacity )
    {
        error_code = ERROR_OVERFLOW;
        error_print( "hash_table_insert" );

        return -1;
    }

    assert( (*ht)->kattr.copy );
    assert( (*ht)->dattr.copy );
    assert( (*ht)->kattr.compare );
    assert( (*ht)->dattr.free );
    assert( (*ht)->kattr.free );

    /* create */
    new_node = (struct _hash_table_node *)calloc( 1, sizeof( struct _hash_table_node ) );
    assert( new_node );
    if( !new_node )
    {
        error_code = ERROR_MEMORY_ALLOCATION_FAILED;
        error_print( "hash_table_insert" );

        return -1;
    }

    /* init */
    new_node->key = (*ht)->kattr.copy( key );
    new_node->data = (*ht)->dattr.copy( data );

    /* insert */
    idx = (*ht)->kattr.hash( key ) % (*ht)->capacity;

    if( (*ht)->hash_table[ idx ] )
    {
        /* probe */
        for( i = 0; i < (*ht)->capacity; ++i )
        {
            if( (*ht)->hash_table[ (idx + i) % (*ht)->capacity ] )
            {
                if( (*ht)->kattr.compare( (*ht)->hash_table[ (idx + i) % (*ht)->capacity ]->key, key ) == 0 )
                {
                    (*ht)->dattr.free( (*ht)->hash_table[ (idx + i) % (*ht)->capacity ]->data );
                    (*ht)->hash_table[ (idx + i) % (*ht)->capacity ]->data = NULL;

                    (*ht)->hash_table[ (idx + i) % (*ht)->capacity ]->data = (*ht)->dattr.copy( data );

                    (*ht)->kattr.free( new_node->key );
                    new_node->key = NULL;
                    (*ht)->dattr.free( new_node->data );
                    new_node->data = NULL;
                    free( new_node );
                    new_node = NULL;

                    break;
                }
            }
            else
            {
                (*ht)->hash_table[ (idx + i) % (*ht)->capacity ] = new_node;
                (*ht)->count++;

                break;
            }
        }
    }
    else 
    {
        (*ht)->hash_table[ idx ] = new_node;
        (*ht)->count++;
    }

    return 0;
}

/**
 * Lookup item in the table
 */
void * hash_table_lookup( const hash_table_t * ht, void * key )
{
    unsigned long idx = 0;
    int i = 0;

    /* check */
    if( !ht )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "hash_table_lookup" );

        return NULL;
    }

    if( !key )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "hash_table_lookup" );

        return NULL;
    }

    assert( ht->kattr.hash );
    assert( ht->kattr.compare );

    /* lookup */
    idx = ht->kattr.hash( key ) % ht->capacity;
    if( ht->hash_table[ idx ] || ht->marker[ idx ] )
    {
        /* probe */
        for( i = 0; i < ht->capacity; ++i )
        {
            if( ht->hash_table[ (idx + i) % ht->capacity ] )
            {
                if( ht->kattr.compare( key, ht->hash_table[ (idx + i) % ht->capacity ]->key ) == 0 )
                {
                    return ht->hash_table[ (idx + i) % ht->capacity ]->data;
                }
            }
        }
    }

    error_code = ERROR_NOT_FOUND;
    error_print( "hash_table_lookup" );

    return NULL;
}

/**
 * Remove item from hash table.
 */
int hash_table_remove( hash_table_t ** ht, void * key )
{
    int idx = 0;
    int i = 0;

    /* check */
    if( !ht )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "hash_table_remove" );

        return -1;
    }

    if( !*ht )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "hash_table_remove" );

        return -1;
    }
    
    if( !key )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "hash_table_remove" );

        return -1;
    }

    if( (*ht)->count <= 0 )
    {
        error_code = ERROR_UNDERFLOW;
        error_print( "hash_table_remove" );

        return -1;
    }

    assert( (*ht)->kattr.free );
    assert( (*ht)->kattr.compare );
    assert( (*ht)->dattr.free );

    /* remove */
    idx = (*ht)->kattr.hash( key ) % (*ht)->capacity;

    if( (*ht)->hash_table[ idx ] || (*ht)->marker[ idx ] )
    {
        for( i = 0; i < (*ht)->capacity; ++i )
        {
            if( (*ht)->hash_table[ (idx + i) % (*ht)->capacity ] )
            {
                if( (*ht)->kattr.compare( (*ht)->hash_table[ (idx + i) % (*ht)->capacity ]->key, key ) == 0 )
                {
                    (*ht)->kattr.free( (*ht)->hash_table[ (idx + i) % (*ht)->capacity ]->key );
                    (*ht)->hash_table[ (idx + i) % (*ht)->capacity ]->key = NULL;

                    (*ht)->dattr.free( (*ht)->hash_table[ (idx + i) % (*ht)->capacity ]->data );
                    (*ht)->hash_table[ (idx + i) % (*ht)->capacity ]->data = NULL;

                    free( (*ht)->hash_table[ (idx + i) % (*ht)->capacity ] );
                    (*ht)->hash_table[ (idx + i) % (*ht)->capacity ] = NULL;

                    (*ht)->count--;

                    if( i == 0 )
                    {
                        (*ht)->marker[ idx ] = 1;
                    }

                    assert( (*ht)->count >= 0 );

                    return 0;
                }
            }
        }
    }

#if 0
    /* find node */
    idx = (*ht)->kattr.hash( key ) % (*ht)->capacity;

    if( (*ht)->hash_table[ idx ] )
    {
        (*ht)->kattr.free( (*ht)->hash_table[ idx ]->key );
        (*ht)->hash_table[ idx ]->key = NULL;

        (*ht)->dattr.free( (*ht)->hash_table[ idx ]->data );
        (*ht)->hash_table[ idx ]->data = NULL;

        free( (*ht)->hash_table[ idx ] );
        (*ht)->hash_table[ idx ] = NULL;

        (*ht)->marker[ idx ] = 1;
        (*ht)->count--;

        assert( (*ht)->count >= 0 );

        return 0;
    }
    else if( (*ht)->marker[ idx ] )
    {
        /* probe */
        for( i = 1; i < (*ht)->capacity; ++i )
        {
            if( (*ht)->kattr.compare( key, (*ht)->hash_table[ (idx + i) % (*ht)->capacity ] ) == 0 )
            {
                (*ht)->kattr.free( (*ht)->hash_table[ (idx + i) % (*ht)->capacity ]->key );
                (*ht)->hash_table[ (idx + i) % (*ht)->capacity ]->key = NULL;

                (*ht)->dattr.free( (*ht)->hash_table[ (idx + i) % (*ht)->capacity ]->data );
                (*ht)->hash_table[ (idx + i) % (*ht)->capacity ]->data = NULL;

                free( (*ht)->hash_table[ (idx + i) % (*ht)->capacity ] );
                (*ht)->hash_table[ (idx + i) % (*ht)->capacity ] = NULL;

                (*ht)->count--;

                assert( (*ht)->count >= 0 );

                return 0;
            }
        }
    }
#endif

    error_code = ERROR_NOT_FOUND;
    error_print( "hash_table_remove" );

    return -1;
}

/**
 * Print a hash table
 */
int hash_table_print( const hash_table_t * ht, FILE * fp )
{
    struct _hash_table_node * current = NULL;
    int nchr = 0;
    int i = 0;

    /* check */
    if( !ht )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "hash_table_print" );

        return -1;
    }

    if( !fp )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "hash_table_print" );

        return -1;
    }

    assert( ht->kattr.print );
    assert( ht->dattr.print );

    /* print */
    nchr += fprintf( fp, "Hash Table (%p)\n", (void *)ht );
    nchr += fprintf( fp, " - count = %d\n", ht->count );
    nchr += fprintf( fp, " - capacity = %d\n", ht->capacity );
    nchr += fprintf( fp, " - load = %f\n", hash_table_load( ht ) );
    nchr += fprintf( fp, " - memory = %d\n", ht->memory );

    for( i = 0; i < ht->capacity; ++i )
    {
        current = ht->hash_table[ i ];
        if( current )
        {
            nchr += ht->kattr.print( current->key, fp );
            nchr += fprintf( fp, " => " );
            nchr += ht->dattr.print( current->data, fp );
            nchr += fprintf( fp, "\n" );
        }
    }

    return nchr;
}

/**
 * Get the hash table's load.
 */
inline double hash_table_load( const hash_table_t * ht )
{
    /* check */
    if( !ht )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "hash_table_load" );

        return -1.0;
    }

    return (double)ht->count / ht->capacity;
}

/**
 * Get the capacity of a hash table.
 */
inline int hash_table_capacity( const hash_table_t * ht )
{
    /* check */
    if( !ht )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "hash_table_capacity" );

        return -1;
    }

    return ht->capacity;
}

/**
 * Get the number of items in a hash table.
 */
inline int hash_table_count( const hash_table_t * ht )
{
    /* check */
    if( !ht )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "hash_table_count" );

        return -1;
    }

    return ht->count;
}

/**
 * Get the memory usage of the hash table.
 */
inline int hash_table_memory( const hash_table_t * ht )
{
    /* check */
    if( !ht )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "hash_table_memory" );

        return -1;
    }

    return ht->memory;
}
