#ifndef EX2_REPO_TESTBUSLINES_H
#define EX2_REPO_TESTBUSLINES_H
// write only between #define EX2_REPO_TESTBUSLINES_H and #endif
// EX2_REPO_TESTBUSLINES_H
#include "sort_bus_lines.h"

/**
 * Perform the algorithm on pointers to structs, comparing strings
 * @param start_sorted
 * @param array_size
 * @param line_org_name
 * @return Success/fail
 */
int binary_search(BusLine * start_sorted, int array_size,
                  char* line_org_name);
/**
 * Tests the distance sorting
 * @param start of sorted array of structs
 * @param end of sorted array of structs
 * @param start_org of array of structs
 * @param end_org of array of structs
 * @return Success/fail
 */
int test_1(BusLine *start, BusLine *end, BusLine * start_org,
           BusLine * end_org);
/**
 * Tests the duration sorting
 * @param start of sorted array of structs
 * @param end of sorted array of structs
 * @param start_org of array of structs
 * @param end_org of array of structs
 * @return Success/fail
 */
int test_2(BusLine *start, BusLine *end, BusLine * start_org,
           BusLine * end_org);
/**
 * Tests the name sorting
 * @param start of sorted array of structs
 * @param end of sorted array of structs
 * @param start_org of array of structs
 * @param end_org of array of structs
 * @return Success/fail
 */
int test_3(BusLine *start, BusLine *end, BusLine * start_org,
           BusLine * end_org);
/**
 * Checks whether the array is actually sorted by distance
 * @param start
 * @param end
 * @return Success/fail
 */
int is_sorted_by_distance (BusLine *start, BusLine *end);

/**
 * Checks whether the array is actually sorted by duration
 * @param start
 * @param end
 * @return Success/fail
 */
int is_sorted_by_duration (BusLine *start, BusLine *end);

/**
 * Checks whether the array is actually sorted by name
 * @param start
 * @param end
 * @return Success/fail
 */
int is_sorted_by_name (BusLine *start, BusLine *end);

/**
 * Checks if the sorted and original arrays contain the same structs
 * @param start_sorted
 * @param end_sorted
 * @param start_original
 * @param end_original
 * @return Success/fail
 */
int is_equal (BusLine *start_sorted,
              BusLine *end_sorted, BusLine *start_original,
              BusLine *end_original);
/**
 * Conclude the results of the tests
 * @param lines_array
 * @param array_size
 * @param start
 * @param end
 * @return 1 for success, 0 else
 */
int is_test_passed(BusLine * lines_array, int array_size, BusLine *start,
                   BusLine *end);

// write only between #define EX2_REPO_TESTBUSLINES_H and #endif
// EX2_REPO_TESTBUSLINES_H
#endif //EX2_REPO_TESTBUSLINES_H
