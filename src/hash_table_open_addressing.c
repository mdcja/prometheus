/* Author: Julian Martinez del Campo
 *
 * This file provides an implementation of a hash_table.h using open
 * addressing.  Collisions are resolved via linear probing.
 *
 * This implementation has an average run time complexity for successful
 * searches: 1 + (a/2), where a is the load on the table.
 *
 * This implementation has an average run time complexity for unsucessful
 * searches is: a, where a is the load on the table.
 *
 * ===========================
 * Operation    | Complexity
 *              |-------------
 *              | Best | Worst
 * -------------|------|------
 * create       | O(1) |
 * destroy      | O(n) |
 * clear        | O(n) |
 * reserve      | O(n) |
 * insert       | O(1) | O(n)
 * lookup       | O(1) | O(n)
 * remove       | O(1) | O(n)
 * print        | O(n) |
 * load         | O(1) |
 * capacity     | O(1) |
 * count        | O(1) |
 * memory       | O(1) |
 * ===========================
 *
 * if _EMBEDDED macro is defined then disable functions that depend on stdio.h
 * and assert.h. This may be useful when programming for embedded systems where
 * standard functionality is expected to fail.
 *
 * if _VERBOSE > 0 then print error messages.
 */
#include <stdlib.h>

#ifndef _EMBEDDED
#include <stdio.h>
#include <assert.h>
#endif /* _EMBEDDED */
#include <string.h>

#include "hash_table.h"
#include "error.h"
#include "attributes.h"

/* Hash table node
 */
struct _hash_table_node {
    void * key;
    void * data;
};

/* A hash table
 */
struct _hash_table_type {
    struct _hash_table_node ** hash_table;  /* the actual hash table */
    int * marker;                           /* mark if position is used */
    int capacity;
    int count;                              /* number of items in the table */
    int memory;                             /* memory of hash table */
    attr_t kattr;                           /* key attributes */
    attr_t dattr;                           /* data attributes */
};

/* Create a new hash table. If kattr or dattr is set to NULL then it will
 * default to manipulating pointers and passing by reference. The hash table
 * created will have a memory space complexity of O(1).
 */
hash_table_t * hash_table_create( const attr_t * kattr, const attr_t * dattr, int capacity )
{
    hash_table_t * new_hash_table = NULL;

    /* check */
    if( capacity < 1 )
    {
        error_code = E_INVALID_CAPACITY;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "hash_table_create: capacity" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return NULL;
    }

    /* create */
#ifndef _EMBEDDED
    new_hash_table = (hash_table_t *)calloc( 1, sizeof( hash_table_t ) );
#else
    new_hash_table = (hash_table_t *)malloc( sizeof( hash_table_t ) );
    memset( new_hash_table, 0, sizeof( hash_table_t ) );
#endif /* _EMBEDDED */
    if( !new_hash_table )
    {
        error_code = E_MEMORY_ALLOCATION_FAILED;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "hash_table_create: new_hash_table" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return NULL;
    }

#ifndef _EMBEDDED
    new_hash_table->hash_table = (struct _hash_table_node **)calloc( 
            capacity, 
            sizeof( struct _hash_table_node ) 
    );
#else
    new_hash_table->hash_table = (struct _hash_table_node **)malloc( 
            capacity * sizeof( struct _hash_table_node ) 
    );
    memset( new_hash_table->hash_table, 0, sizeof( capacity * sizeof( struct _hash_table_node ) ) );
#endif /* _EMBEDDED */
    if( !new_hash_table->hash_table )
    {
        error_code = E_MEMORY_ALLOCATION_FAILED;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "hash_table_create: new_hash_table->hash_table" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return NULL;
    }

#ifndef _EMBEDDED
    new_hash_table->marker = (int *)calloc( capacity, sizeof( int ) );
#else
    new_hash_table->marker = (int *)malloc( capacity * sizeof( int ) );
    memset( new_hash_table->marker, 0, sizeof( capacity * sizeof( int ) ) );
#endif /* _EMBEDDED */
    if( !new_hash_table->marker )
    {
        error_code = E_MEMORY_ALLOCATION_FAILED;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "hash_table_create: new_hash_table->marker" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return NULL;
    }

    /* init */
    new_hash_table->capacity = capacity;
    new_hash_table->count = 0;
    new_hash_table->memory = sizeof( hash_table_t ) + 
        capacity * sizeof( struct _hash_table_node * );

    if( kattr == NULL )
    {
        attr_init( &new_hash_table->kattr );
    }
    else
    {
        new_hash_table->kattr = *kattr;
    }

    if( dattr == NULL )
    {
        attr_init( &new_hash_table->dattr );
    }
    else
    {
        new_hash_table->dattr = *dattr;
    }

#ifndef _EMBEDDED
    assert( new_hash_table->kattr.copy );
    assert( new_hash_table->kattr.free );
    assert( new_hash_table->kattr.compare );
    assert( new_hash_table->kattr.print );
    assert( new_hash_table->kattr.hash );
    assert( new_hash_table->dattr.copy );
    assert( new_hash_table->dattr.free );
    assert( new_hash_table->dattr.print );
#endif /* _EMBEDDED */
    
    return new_hash_table;
}

/* Destroy a hash table.
 */
int hash_table_destroy( hash_table_t ** ht )
{
    /* check */
    if( ht == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "hash_table_destroy: ht" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    if( *ht == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "hash_table_destroy: *ht" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    /* deallocate all items in the hash table */
    if( hash_table_clear( ht ) != 0 )
    {
#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "hash_table_destroy: hash_table_clear" );
        return -1;
#endif /* _VERBOSE */
#endif /* _EMBEDDED */
    }

    /* deallocate */
    free( (*ht)->hash_table );
    (*ht)->hash_table = NULL;

    free( *ht );
    *ht = NULL;

    return 0;
}

/* Clear a hash table. Has run time complexity of O(n).
 */
int hash_table_clear( hash_table_t ** ht )
{
    struct _hash_table_node * temp = NULL;
    int i = 0;

    /* check */
    if( !ht )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "hash_table_clear" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    if( !*ht )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "hash_table_clear" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    /* ignore empty hash table */
    if( (*ht)->count == 0 )
    {
        return 0;
    }

#ifndef _EMBEDDED
    /* key and data attributes should never be NULL */
    assert( (*ht)->kattr.free );
    assert( (*ht)->dattr.free );
#endif /* _EMBEDDED */

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

/* Resize a hash table. Has runtime complexity of O(n).
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
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "hash_table_reserve" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    if( !*ht )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "hash_table_reserve" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    if( capacity < (*ht)->count )
    {
        error_code = E_INVALID_CAPACITY;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "hash_table_reserve" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

#ifndef _EMBEDDED
    assert( (*ht)->kattr.hash );
#endif /* _EMBEDDED */

    /* create */
#ifndef _EMBEDDED
    new_hash_table = (struct _hash_table_node **)calloc( 
            capacity, 
            sizeof( struct _hash_table_node * ) );
#else
    new_hash_table = (struct _hash_table_node **)malloc(
            capacity * sizeof( struct _hash_table_node * ) );
    memset( new_hash_table, 0, capacity * sizeof( struct _hash_table_node * ) );
#endif /* _EMBEDDED */
    if( !new_hash_table )
    {
        error_code = E_MEMORY_ALLOCATION_FAILED;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "hash_table_reserve" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

#ifndef _EMBEDDED
    new_marker = (int *)calloc( capacity, sizeof( int ) );
#else
    new_marker = (int *)malloc( capacity * sizeof( int ) );
    memset( new_marker, 0, capacity * sizeof( int ) );
#endif /* _EMBEDDED */
    if( !new_marker )
    {
        error_code = E_MEMORY_ALLOCATION_FAILED;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "hash_table_reserve" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    /* copy */
    for( i = 0; i < (*ht)->capacity; ++i )
    {
        if( (*ht)->hash_table[ i ] )
        {
            /* re-insert */
            idx = (*ht)->kattr.hash( (*ht)->hash_table[ i ]->key, 0 ) % capacity;
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

/* Insert/Update item into a hash table. If the item already exists in the hash
 * table then it is replaced with the new item. Collisions are resolved using
 * open addressing.  This implementation has an average run time complexity for
 * successful searches: 1 + (a/2), where a is the load on the table.  This
 * implementation has an average run time complexity for unsucessful searches
 * is: a, where a is the load on the table.
 */
int hash_table_insert( hash_table_t ** ht, void * key, void * data )
{
    struct _hash_table_node * new_node = NULL;
    int idx = 0;
    int i = 0;

    /* check */
    if( ht == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "hash_table_insert: ht" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    if( *ht == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "hash_table_insert: *ht" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    if( key == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "hash_table_insert: key" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    if( data == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "hash_table_insert: data" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    if( (*ht)->count >= (*ht)->capacity )
    {
        error_code = E_OVERFLOW;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "hash_table_insert: (*ht)->capacity" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

#ifndef _EMBEDDED
    assert( (*ht)->kattr.copy );
    assert( (*ht)->dattr.copy );
    assert( (*ht)->kattr.compare );
    assert( (*ht)->dattr.free );
    assert( (*ht)->kattr.free );
#endif /* _EMBEDDED */

    /* create */
#ifndef _EMBEDDED
    new_node = (struct _hash_table_node *)calloc( 1, sizeof( struct _hash_table_node ) );
#else
    new_node = (struct _hash_table_node *)malloc(
            sizeof( struct _hash_table_node ) );
    memset( new_node, 0, sizeof( struct _hash_table_node ) );
#endif /* _EMBEDDED */
    if( new_node == NULL )
    {
        error_code = E_MEMORY_ALLOCATION_FAILED;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "hash_table_insert: new_node" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return -1;
    }

    /* init */
    new_node->key = (*ht)->kattr.copy( key );
    new_node->data = (*ht)->dattr.copy( data );

    /* insert */
    idx = (*ht)->kattr.hash( key, 0 ) % (*ht)->capacity;

    if( (*ht)->hash_table[ idx ] )
    {
        /* probe */
        for( i = 0; i < (*ht)->capacity; ++i )
        {
            if( (*ht)->hash_table[ (idx + i) % (*ht)->capacity ] )
            {
                /* need to update */
                if( (*ht)->kattr.compare( (*ht)->hash_table[ (idx + i) % (*ht)->capacity ]->key, key ) == 0 )
                {
                    /* free existing data */
                    (*ht)->dattr.free( (*ht)->hash_table[ (idx + i) % (*ht)->capacity ]->data );
                    (*ht)->hash_table[ (idx + i) % (*ht)->capacity ]->data = NULL;

                    /* insert new data */
                    (*ht)->hash_table[ (idx + i) % (*ht)->capacity ]->data = (*ht)->dattr.copy( data );

                    /* destroy unused node */
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

/* Lookup item in the table. Collisions are resolved using open addressing.
 * This implementation has an average run time complexity for successful
 * searches: 1 + (a/2), where a is the load on the table.  This implementation
 * has an average run time complexity for unsucessful searches is: a, where a
 * is the load on the table.
 */
void * hash_table_lookup( const hash_table_t * ht, void * key )
{
    unsigned long idx = 0;
    int i = 0;

    /* check */
    if( ht == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "hash_table_lookup: ht" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return NULL;
    }

    if( key == NULL )
    {
        error_code = E_NULL_PARAMETER;

#ifndef _EMBEDDED
#if _VERBOSE > 0
        error_print( "hash_table_lookup: key" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

        return NULL;
    }

#ifndef _EMBEDDED
    assert( ht->kattr.hash );
    assert( ht->kattr.compare );
#endif /* _EMBEDDED */

    /* lookup */
    idx = ht->kattr.hash( key, 0 ) % ht->capacity;
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

    error_code = E_NOT_FOUND;

#ifndef _EMBEDDED
#if _VERBOSE > 0
    error_print( "hash_table_lookup" );
#endif /* _VERBOSE */
#endif /* _EMBEDDED */

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
