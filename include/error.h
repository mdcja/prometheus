/**
 * This file provides error reporting
 *
 * @brief Provides error reporting functions
 * @author Julian Martinez del Campo
 * @license GNU Lesser General Public License (LGPL) https://www.gnu.org/copyleft/lesser.html
 *
 * This file provides a list of errors that may occur when using the abstract
 * data types, or any other data structures or problems.
 */
#ifndef _ERROR_H_
#define _ERROR_H_

#include <stdio.h>

/**
 * The types of errors that may occur
 */
typedef enum _errors {
    ERROR_UNKNOWN = 1,
    ERROR_MEMORY_ALLOCATION_FAILED,
    ERROR_NULL_PARAMETER,
    ERROR_NOT_FOUND,
    ERROR_INVALID_CAPACITY,
    ERROR_NOT_IMPLEMENTED,
    ERROR_OVERFLOW,
    ERROR_UNDERFLOW
} error_t;

/**
 * The global error code that is used to get error code from
 *
 * @warning error code is not initialized automatically. This value is tracked
 * by the programmer.
 */
error_t error_code;

/**
 * Print an error message.
 *
 * @post Error message is printed to stderr.
 * @param msg an error message.
 * @return the number of characters printed to screen, return -1 on error.
 */
int error_print( const char * msg );

/**
 * Print an error message to a stream.
 *
 * @param fp a stream to print message to.
 * @param msg a message to print.
 * @return the number of characters printed to stream return -1 on error.
 */
int error_print_message( FILE * fp, char * msg );

#endif /* _ERROR_H_ */
