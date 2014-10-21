/* Author: Julian Martinez del Campo
 *
 * This file provides basic error types, a variable to keep track of errors, as
 * well as error printing functions. This file was inspired and is very similar
 * to the role of errno.h.
 *
 * The _EMBEDDED macro can be defined to exclude all functions that use stdio.h
 * to print messages, which is useful if programming on an embedded system.
 */
#ifndef _ERROR_H_
#define _ERROR_H_

#ifndef _EMBEDDED
#include <stdio.h>
#endif /* _EMBEDDED */

/* Error types
 */
typedef enum _errors {
    E_UNKNOWN = -1,
    E_NO_ERROR = 0,
    E_MEMORY_ALLOCATION_FAILED,
    E_NULL_PARAMETER,
    E_NOT_FOUND,
    E_INVALID_CAPACITY,
    E_NOT_IMPLEMENTED,
    E_OVERFLOW,
    E_UNDERFLOW
} error_t;

/* This variable is used by the programmer to set errors (much like errno).
 */
error_t error_code;

#ifndef _EMBEDDED
/* Get the error message associated with the error. This function returns a
 * null terminated character array with the error message.
 */
char * error_get_message( error_t error );

/* Print an error message to stdout. The message printed is a combination of
 * the error message associated with the error, and an additional custom
 * message provided as input. This function returns the number of characters
 * printed, else -1 to indicate an error.
 */
int error_print( const char * message );

/* Print an error message to a file. The message printed is a combination of
 * the error message associated with the error, and an additional custom
 * message provided as input. This function returns the number of characters
 * printed, else -1 to indicate an error.
 */
int error_print_message( FILE * fp, char * message );
#endif /* _EMBEDDED */

#endif /* _ERROR_H_ */
