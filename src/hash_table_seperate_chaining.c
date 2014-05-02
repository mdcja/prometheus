/**
 * This file provides an implementation of a hash table using seperate chaining
 * with linked lists.
 *
 * \brief Provides a hash table ADT implemented using seperate chaining.
 * \author Julian Martinez del Campo
 * \license GNU Lesser General Public License (LGPL) https://www.gnu.org/copyleft/lesser.html
 *
 * While this implementation allows for the hash table to be filled beyond its
 * normal capacity, performance will degrade linearly.
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
 * reserve      | O(n) |
 * insert       | O(1) | O(n)
 * lookup       | O(1) | O(n)
 * remove       | O(1) | O(n)
 * print        | O(n) |
 * load         | O(1) |
 * capacity     | O(1) |
 * count        | O(1) |
 * memory       | O(1) |
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "hash_table.h"
#include "error.h"
#include "attributes.h"

/**
 * Linked list node
 */
struct _hash_table_node {
    void * key;
    void * data;
    struct _hash_table_node * next;
};

/**
 * A hash table
 */
struct _hash_table_type {
    struct _hash_table_node ** hash_table;
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
    struct _hash_table_node * current = NULL;
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

    assert( (*ht)->kattr.free );
    assert( (*ht)->dattr.free );

    if( (*ht)->count == 0 )
    {
        return 0;
    }

    for( i = 0; i < (*ht)->capacity; ++i )
    {
        /* destroy each chain */
        for( current = (*ht)->hash_table[ i ]; current != NULL; )
        {
            temp = current;
            current = current->next;

            /* free */
            (*ht)->kattr.free( temp->key );
            temp->key = NULL;

            (*ht)->dattr.free( temp->data );
            temp->data = NULL;

            free( temp );
            temp = NULL;

            (*ht)->count--;
            (*ht)->memory -= sizeof( struct _hash_table_node );
        }
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
    struct _hash_table_node * current = NULL;
    struct _hash_table_node * temp = NULL;
    int idx = 0;
    int i = 0;

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
    new_hash_table = (struct _hash_table_node **)calloc(capacity, sizeof(struct _hash_table_node *));
    assert( new_hash_table );
    if( !new_hash_table )
    {
        error_code = ERROR_MEMORY_ALLOCATION_FAILED;
        error_print( "hash_table_reserve" );

        return -1;
    }

    /* copy */
    for( i = 0; i < (*ht)->capacity; ++i )
    {
        /* follow each chain */
        for( current = (*ht)->hash_table[ i ], temp = NULL; current != NULL; )
        {
            temp = current;
            current = current->next;

            idx = (*ht)->kattr.hash( temp->key ) % capacity;

            /* insert into new table */
            if( new_hash_table[ idx ] == NULL )
            {
                temp->next = NULL;
            } 
            else
            {
                temp->next = new_hash_table[ idx ];
            }
            new_hash_table[ idx ] = temp;
        }
    }

    /* replace */
    old_hash_table = (*ht)->hash_table;
    (*ht)->hash_table = new_hash_table;
    (*ht)->capacity = capacity;
    (*ht)->memory = sizeof( hash_table_t ) + capacity * sizeof( struct _hash_table_node * ) + (*ht)->count * sizeof( struct _hash_table_node );

    /* free */
    free( old_hash_table );
    old_hash_table = NULL;

    return 0;
}

/**
 * Insert/Update item into a hash table.
 */
int hash_table_insert( hash_table_t ** ht, void * key, void * data )
{
    struct _hash_table_node * new_node = NULL;
    struct _hash_table_node * current = NULL;
    int idx = 0;

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

    assert( (*ht)->kattr.copy );
    assert( (*ht)->dattr.copy );
    assert( (*ht)->kattr.compare );
    assert( (*ht)->dattr.free );

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
    new_node->next = NULL;

    /* check for conflicts */
    idx = (*ht)->kattr.hash( key ) % (*ht)->capacity;

    for( current = (*ht)->hash_table[ idx ]; current != NULL; current = current->next )
    {
        /* update */
        if( (*ht)->kattr.compare( current->key, key ) == 0 )
        {
            (*ht)->dattr.free( current->data );
            current->data = NULL;

            current->data = (*ht)->dattr.copy( data );

            (*ht)->kattr.free( new_node->key );
            new_node->key = NULL;
            (*ht)->dattr.free( new_node->data );
            new_node->data = NULL;
            free( new_node );
            new_node = NULL;

            break;
        }
    }

    if( current == NULL )
    {
        new_node->next = (*ht)->hash_table[ idx ];
        (*ht)->hash_table[ idx ] = new_node;

        (*ht)->count++;
        (*ht)->memory += sizeof( struct _hash_table_node );
    }

    return 0;
}

/**
 * Lookup item in the table
 */
void * hash_table_lookup( const hash_table_t * ht, void * key )
{
    struct _hash_table_node * current = NULL;
    int idx = 0;

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

    /* lookup */
    idx = ht->kattr.hash( key ) % ht->capacity;
    for( current = ht->hash_table[idx]; current != NULL; current = current->next )
    {
        if( ht->kattr.compare( current->key, key ) == 0 )
        {
            return current->data;
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
    struct _hash_table_node * current = NULL;
    struct _hash_table_node * prev = NULL;
    int idx = 0;

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

    /* find node */
    idx = (*ht)->kattr.hash( key ) % (*ht)->capacity;
    for( current = (*ht)->hash_table[idx], prev = NULL; current != NULL; current = current->next )
    {
        if( (*ht)->kattr.compare( current->key, key ) == 0 )
        {
            /* remove item */
            if( prev == NULL )
            {
                (*ht)->hash_table[idx] = current->next;
            }
            else
            {
                prev->next = current->next;
            }

            /* deallocate */
            (*ht)->kattr.free( current->key );
            current->key = NULL;

            (*ht)->dattr.free( current->data );
            current->data = NULL;

            free( current );
            current = NULL;

            (*ht)->count--;
            (*ht)->memory -= sizeof( struct _hash_table_node );

            assert( (*ht)->count >= 0 );

            return 0;
        }

        prev = current;
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
        for( current = ht->hash_table[i]; current != NULL; current = current->next )
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
