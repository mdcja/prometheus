/**
 * This file provides misc math functions.
 *
 * \brief provides generic math functions.
 * \author Julian Martinez del Campo
 * \license GNU Lesser General Public License (LGPL) https://www.gnu.org/copyleft/lesser.html
 *
 * This file provides math functions that are often used and/or not shipped
 * with the standard math libraries available.
 */
#ifndef _MATH_H_
#define _MATH_H_

/**
 * Generate a prime number.
 *
 * This function generates a prime number that is just below the desired limit.
 *
 * \param limit the limit at which prime numbers will not be generated. Limit
 *  must be greater than 2.  
 * \return the last prime number under limit.
 */
unsigned long generate_prime( unsigned int limit );

/**
 * Generate a random number.
 *
 * This function generates a random number between min and max.
 *
 * \param min the minimum number value.
 * \param max the maximum number value.
 * \return a random number.
 */
int random_number( int min, int max );

#endif /* _MATH_H_ */
