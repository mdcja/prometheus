/**
 * This file provides error reporting implementation
 *
 * @brief Provides an implementation of errors
 * @author Julian Martinez del Campo
 * @license GNU Lesser General Public License (LGPL) https://www.gnu.org/copyleft/lesser.html
 *
 * This file implements error reporting functions.
 */
#include <stdio.h>

#include "error.h"

#define MAX_STRLEN 256

/**
 * Get the error name
 *
 * This function returns a string containing the name of the error that occured
 *
 * @param error the error number 
 * @return a string containing the error that occured
 */
static char * error_name( error_t error )
{
    static char * error_names[ MAX_STRLEN ] = {
        "no error",
        "unknown",
        "memory allocation failed",
        "null parameter",
        "not found",
        "invalid capacity",
        "not implemented",
        "overflow",
        "underflow"
    };
        
    switch( error )
    {
    case ERROR_NO_ERROR:
        return error_names[ ERROR_NO_ERROR ];
    case ERROR_MEMORY_ALLOCATION_FAILED:
        return error_names[ ERROR_MEMORY_ALLOCATION_FAILED - 1 ];
    case ERROR_NULL_PARAMETER:
        return error_names[ ERROR_NULL_PARAMETER - 1 ];
    case ERROR_NOT_FOUND:
        return error_names[ ERROR_NOT_FOUND - 1 ];
    case ERROR_INVALID_CAPACITY:
        return error_names[ ERROR_INVALID_CAPACITY - 1 ];
    case ERROR_NOT_IMPLEMENTED:
        return error_names[ ERROR_NOT_IMPLEMENTED - 1 ];
    case ERROR_OVERFLOW:
        return error_names[ ERROR_OVERFLOW - 1 ];
    case ERROR_UNDERFLOW:
        return error_names[ ERROR_UNDERFLOW - 1 ];
    default:
        return error_names[ ERROR_UNKNOWN - 1 ];
    }
}

/**
 * Get error message
 */
inline char * error_get_message( int error )
{
    return error_name( error );
}

/**
 * Print the error that occured
 */
int error_print( const char * msg )
{
    int nchr = 0;

#ifndef NERROR
    nchr += fprintf( stderr, "ERROR: (%d) %s\n", error_code, msg );
#ifdef VERBOSE
    nchr += fprintf( stderr, " - %s\n", error_name( error_code ) );
    nchr += fprintf( stderr, " - error code = %d\n", error_code );
#endif /* VERBOSE */
#endif /* NERROR */
    return nchr;
}

/**
 * Print the error that occured with a message
 */
int error_print_message( FILE * fp, char * msg )
{
    int nchr = 0;
#ifndef NERROR
    nchr += fprintf( fp, "ERROR: %s\n", msg );
#ifdef VERBOSE 
    nchr += fprintf( fp, " - %s\n", error_name( error_code ) );
    nchr += fprintf( fp, " - error code = %d\n", error_code );
#endif /* VERBOSE */
#endif /* NERROR */
    return nchr;
}
