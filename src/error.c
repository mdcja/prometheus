/* Author: Julian Martinez del Campo
 *
 * This file implements error.h
 */
#include "error.h"

#define MAX_STRLEN 256  /* the maximum string length */

/* Get the name of an error. Returns a null terminated character array
 * containing the error type.
 */
static char * error_name( error_t error )
{
    char * error_name = NULL;

    /* order MUST correspond to the order of errors in error_t */
    static char * error_names[ MAX_STRLEN ] = {
        "unknown",
        "no error",
        "memory allocation failed",
        "null parameter",
        "not found",
        "invalid capacity",
        "not implemented",
        "overflow",
        "underflow"
    };
        
    /* assign error message with error */
    switch( error )
    {
    case E_NO_ERROR:
        error_name = error_names[ E_NO_ERROR ];
        break;

    case E_MEMORY_ALLOCATION_FAILED:
        error_name = error_names[ E_MEMORY_ALLOCATION_FAILED ];
        break;

    case E_NULL_PARAMETER:
        error_name = error_names[ E_NULL_PARAMETER ];
        break;

    case E_NOT_FOUND:
        error_name = error_names[ E_NOT_FOUND ];
        break;

    case E_INVALID_CAPACITY:
        error_name = error_names[ E_INVALID_CAPACITY ];
        break;

    case E_NOT_IMPLEMENTED:
        error_name = error_names[ E_NOT_IMPLEMENTED ];
        break;

    case E_OVERFLOW:
        error_name = error_names[ E_OVERFLOW ];
        break;

    case E_UNDERFLOW:
        error_name = error_names[ E_UNDERFLOW ];
        break;

    default:
        error_name = error_names[ E_UNKNOWN ];
        break;
    }

    return error_name;
}

inline char * error_get_message( error_t error )
{
    return error_name( error );
}

/* Print error messages in the following format:
 *  ERROR: (<error code>) <error name> : <error messsage>
 */
int error_print( const char * msg )
{
    return printf( "ERROR: (%d) %s : %s\n", error_code, error_name( error_code), msg );
}

/* Print error messages in the following format:
 *  ERROR: (<error code>) <error name> : <error messsage>
 */
int error_print_message( FILE * fp, char * msg )
{
    return fprintf( fp, "ERROR: (%d) %s : %s\n", error_code, error_name( error_code), msg );
}
