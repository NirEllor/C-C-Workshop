#include "test_bus_lines.h"
#include "sort_bus_lines.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//TODO add implementation here


int is_sorted_by_distance (BusLine *start, BusLine *end)
{
    // now I will know whether that the new array is sorted
    quick_sort(start, end, DISTANCE);
    BusLine * left_element = start;
    BusLine * right_element = start + 1;
    while (right_element < end)
    {
        if (left_element ->distance > right_element -> distance)
        {
            return 0;
        }
        ++right_element;
        ++left_element;
    }
    return 1;
}

int is_sorted_by_duration (BusLine *start, BusLine *end)
{
    // now I will know whether that the new array is sorted
    quick_sort(start, end, DURATION);
    BusLine * left_element = start;
    BusLine * right_element = start + 1;
    while (right_element < end)
    {
        if (left_element ->duration > right_element -> duration)
        {
            return 0;
        }
        ++right_element;
        ++left_element;
    }
    return 1;
}

int is_sorted_by_name (BusLine *start, BusLine *end)
{
    bubble_sort(start, end);
    BusLine * right_element = start + 1;
    BusLine * left_element = start;
    while (right_element < end - 1)
    {
        if (strcmp(left_element -> name, right_element -> name) > 0)
        {
            return 0;
        }
        ++right_element;
        ++left_element;
    }
    return 1;
}

int binary_search(BusLine * start_sorted, int array_size,
                  char* line_org_name)
{
    int left = 0, right = array_size - 1;
    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        int cmp = strcmp((start_sorted + mid) -> name, line_org_name);
        if (cmp == 0)
        {
            return EXIT_SUCCESS;
        }
        else if (cmp < 0)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    return EXIT_FAILURE;
}

int is_equal (BusLine *start_sorted, BusLine *end_sorted,
              BusLine *start_original,
              BusLine *end_original)
              {
    int array_sorted_size = 0;
    int array_org_size = 0;
    while (start_sorted < end_sorted)
    {
        start_sorted++;
        array_sorted_size++;
    }
    while (start_original < end_original)
    {
        start_original++;
        array_org_size++;
    }
    if (array_sorted_size != array_org_size)
    {
        return 0;
    }
    if (is_sorted_by_name(start_sorted, end_sorted) == 1)
    {
        for (int i = 0; i < array_org_size; ++i)
        {
            int is_org_in_sorted =
                    binary_search(start_sorted,
                                  array_sorted_size,
                                  (start_original + i) -> name);
            if (is_org_in_sorted == EXIT_FAILURE)
            {
                return 0;

            }
        }
        return 1;
    }
    else
    {
        for (int i = 0; i < array_sorted_size; ++i)
        {
            for (int j = 0; j < array_sorted_size; ++j)
            {
                if (strcmp((start_sorted + i) -> name,
                           (start_original + i) -> name) == 0)
                {
                    break; // we found array_org[i] in array_sorted[i]
                }
            }
            return 0; // we haven't found array_org[i] in array_sorted[i]
        }
        return 1; // we found all array_org in array_sorted
    }
}

int test_1(BusLine *start, BusLine *end, BusLine * start_org,
           BusLine * end_org)
{
    if (is_sorted_by_distance(start, end) == 0)
    {
        fprintf(stdout, "TEST 1 FAILED: Sorted by distance\n");
        return 0;
    }
    else
    {
        fprintf(stdout, "TEST 1 PASSED: Sorted by distance\n");

    }
    if (is_equal(start, end, start_org,
                 end_org) == 0)
    {
        fprintf(stdout, "TEST 2 FAILED: New array is not in"
                        "the same size\n");
        return 0;
    }
    else
    {
        fprintf(stdout, "TEST 2 PASSED: New array is in"
                        "the same size\n");
    }
    return 1;
}

int test_2(BusLine *start, BusLine *end, BusLine * start_org,
           BusLine * end_org)
{
    if (is_sorted_by_duration(start, end) == 0)
    {
        fprintf(stdout, "TEST 3 FAILED: Sorted by duration\n");
        return 0;
    }
    else
    {
        fprintf(stdout, "TEST 3 PASSED: Sorted by duration\n");

    }
    if (is_equal(start, end, start_org,
                 end_org) == 0)
    {
        fprintf(stdout, "TEST 4 FAILED: New array is not in"
                        "the same size\n");
        return 0;
    }
    else
    {
        fprintf(stdout, "TEST 4 PASSED: New array is in"
                        "the same size\n");
    }
    return 1;
}

int test_3(BusLine *start, BusLine *end, BusLine * start_org,
           BusLine * end_org)
{
    if (is_sorted_by_name(start, end) == 0)
    {
        fprintf(stdout, "TEST 5 FAILED: Sorted by name\n");
        return 0;
    }
    else
    {
        fprintf(stdout, "TEST 5 PASSED: Sorted by name\n");

    }
    if (is_equal(start, end, start_org,
                 end_org) == 0)
    {
        fprintf(stdout, "TEST 6 FAILED: New array is not in"
                        "the same size\n");
        return 0;
    }
    else
    {
        fprintf(stdout, "TEST 6 PASSED: New array is in"
                        "the same size\n");
    }
    return 1;
}
int is_test_passed(BusLine * lines_array, int array_size, BusLine *start,
                   BusLine *end)
{
    if (lines_array == NULL)
    {
        return 0;
    }
    BusLine * lines_array_cpy = malloc(array_size * sizeof(BusLine));
    if (!lines_array_cpy)
    {
        return 0;
    }
    memcpy(lines_array_cpy, lines_array,
           array_size * sizeof(BusLine));

    BusLine * start_org = lines_array_cpy;
    BusLine * end_org = (array_size > 1) ? lines_array_cpy + array_size:
            lines_array_cpy;

    if (test_1(start, end, start_org, end_org) == 0)
    {
        free(lines_array_cpy);
        lines_array_cpy = NULL;
        return 0;
    }
    if (test_2(start, end, start_org, end_org) == 0)
    {
        free(lines_array_cpy);
        lines_array_cpy = NULL;
        return 0;
    }
    if (test_3(start, end, start_org, end_org) == 0)
    {
        free(lines_array_cpy);
        lines_array_cpy = NULL;
        return 0;
    }

    free(lines_array_cpy);
    lines_array_cpy = NULL;
    return 1;
}

