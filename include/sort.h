#ifndef _SORT_H_
#define _SORT_H_

#include "attributes.h"

/* Insertion sort. Sorts an array of data according to the attributes 
 * value. All data must be the same type. Returns 0 on success, else return -1.
 *
 * NOTE: Run time efficiency
 *  best    : O(n)
 *  average : O(n^2)
 *  worst   : O(n^2)
 */
int sort_insertion( const attr_t * attr, void * data_array );

/* Median sort. Sorts an array of data according to the attributes value.
 * All data must be the same type. Returns 0 on success, else returns -1.
 * 
 * NOTE: Run time efficiency
 *  best    : O(n log n)
 *  average : O(n log n)
 *  worst   : O(n^2)
 */
int sort_median( const attr_t * attr, void * data_array );

/* Quicksort. Sorts an array of data according to the attributes value.
 * All data must be the same type. Returns 0 on success, else returns -1.
 *
 * NOTE: Run time efficiency
 *  best    : O(n log n)
 *  average : O(n log n)
 *  worst   : O(n^2)
 */
int sort_quicksort( const attr_t * attr, void * data_array );

/* Selection sort. Sorts an array of data according to the attributes value.
 * All data must be the same type. Returns 0 on success, else returns -1.
 *
 * NOTE: Run time efficiency
 *  best    : O(n^2)
 *  average : O(n^2)
 *  worst   : O(n^2)
 */
int sort_selection( const attr_t * attr, void * data_array );

/* Heap sort. Sorts an array of data according to the attributes value.
 * All data must be the same type. Returns 0 on success, else returns -1.
 *
 * NOTE: Run time efficiency
 *  best    : O(n log n)
 *  average : O(n log n)
 *  worst   : O(n log n)
 */
int sort_heap( const attr_t * attr, void * data_array );

/* Counting sort. Sorts an array of data according to the attributes value.
 * All data must be the same type. Returns 0 on success, else returns -1.
 *
 * NOTE: Run time efficiency
 *  best    : O(n)
 *  average : O(n)
 *  worst   : O(n)
 */
int sort_counting( const attr_t * attr, void * data_array );

/* Bucket sort. Sorts an array of data according to the attributes value.
 * All data must be the same type. Returns 0 on success, else returns -1.
 *
 * NOTE: Run time efficiency
 *  best    : O(n)
 *  average : O(n)
 *  worst   : O(n)
 */
int sort_bucket( const attr_t * attr, void * data_array );

#endif /* _SORT_H_ */
