/* Author: Julian Martinez del Campo
 *
 * This file provides some commonly used math routines.
 */
#ifndef _MATH_H_
#define _MATH_H_

/* Generate a prime number. This function returns the closest prime number
 * less than the the limit specified, or else it returns 0 if unsuccessful or 
 * on error.
 */
unsigned long generate_prime( unsigned int limit );

/* Generate a random number. This function returns a random number between min
 * and max.
 */
int random_number( int min, int max );

#endif /* _MATH_H_ */
