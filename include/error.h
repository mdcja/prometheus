/**
 * \brief Provides error reporting functions
 * \author Julian Martinez del Campo <mdcja@outlook.com>
 * \page error Error Handler
 *
 * \section license GNU LGPLv3
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 *
 * \section error_intro Introduction
 *
 * This file provides a list of errors that may occur during execution.
 *
 * \section error_usage Usage
 *
 * The global variable error_code contains the current error state. It must be
 * initialized by the programmer.
 *
 * If an error occurs, then error_code is set, and can be checked against
 * expected behavior.
 */
#ifndef _ERROR_H_
#define _ERROR_H_

#include <stdio.h>

/**
 * The types of errors that may occur
 */
typedef enum Errors {
    ERROR_NO_ERROR = 0,
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
 * \warning error code is not initialized automatically.
 */
error_t error_code;

/**
 * Get error message
 *
 * \param error_code the error number.
 * \return a null terminated string containing the error message.
 */
char * error_get_message( int error );

/**
 * Print an error message.
 *
 * \post Error message is printed to stderr.
 * \param msg an error message.
 * \return The number of characters printed to screen, else return -1 on error.
 */
int error_print( const char * msg );

/**
 * Print an error message to a stream.
 *
 * \post Error message is printed to fp.
 * \param fp a stream to print message to.
 * \param msg a message to print.
 * \return The number of characters printed to stream, else return -1 on error.
 */
int error_print_message( FILE * fp, char * msg );

#endif /* _ERROR_H_ */
