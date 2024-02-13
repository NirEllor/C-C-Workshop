#ifndef EX2_REPO_SORTBUSLINES_H
#define EX2_REPO_SORTBUSLINES_H
// write only between #define EX2_REPO_SORTBUSLINES_H and #endif
// EX2_REPO_SORTBUSLINES_H
#include <string.h>
#define NAME_LEN 21
/**
 * TODO add documentation
 */
typedef struct BusLine
{
    char name[NAME_LEN];
    int distance, duration;
} BusLine;

typedef enum SortType
{
    DISTANCE,
    DURATION
} SortType;


/**
 * Performs the algorithm on pointers to structs, comparing strings
 * @param start of array of structs
 * @param end of array of structs
 */
void bubble_sort (BusLine *start, BusLine *end);

/**
 * Performs the algorithm on pointers to structs, comparing ints
 * @param start of array of structs
 * @param end of array of structs
 */
void quick_sort (BusLine *start, BusLine *end, SortType sort_type);

/**
 * Arranges a pivot element of a given array so each element that is
 * smaller will to the left of the pivot and each element that is greater
 * will to the right of the pivot
 * @param start of array of structs
 * @param end of array of structs
 */
BusLine *partition (BusLine *start, BusLine *end, SortType sort_type);
// write only between #define EX2_REPO_SORTBUSLINES_H and #endif
// EX2_REPO_SORTBUSLINES_H
#endif //EX2_REPO_SORTBUSLINES_H
