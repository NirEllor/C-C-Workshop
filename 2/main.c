#include "sort_bus_lines.h"
#include "test_bus_lines.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define TEST "test"
#define BY_DURATION "by_duration"
#define BY_DISTANCE "by_distance"
#define BY_NAME "by_name"
#define MAX_INT_DIGITS 6
#define MAX_LINE_INFO 61
#define MAX_LINE_NAME 21
#define MIN_LOWER_LETTER 97
#define MAX_LOWER_LETTER 122
#define MIN_NAME_LINE_NUM 48
#define MAX_NAME_LINE_NUM 57
#define MIN_LINE_DISTANCE 0
#define MAX_LINE_DISTANCE 1000
#define MIN_LINE_DURATION 10
#define MAX_LINE_DURATION 100
#define NUMBER__INFO_PARSED 3

/**
 * checks user input regarding the number of lines
 * @param num_of_lines Here the corrected num_of_lines will be stored
 * @return Success/Fail
 */
int ask_num_of_lines(int* num_of_lines)
{
    char num_lines_str[MAX_INT_DIGITS];
    int num_lines = 0;
    fprintf(stdout,"Enter number of lines. Then enter\n");
    char* num_lines_get = fgets(num_lines_str, MAX_INT_DIGITS,
                                stdin);
    if (!num_lines_get)
    {
        fprintf(stdout, "ERROR: fgets failed in number"
                        "of lines.\n");
        return EXIT_FAILURE;
    }

    int num_lines_scan = sscanf(num_lines_get, "%d",
                                &num_lines);
    if (!num_lines_scan)
    {
        fprintf(stdout, "ERROR: sscanf failed in number"
                        "of lines.\n");
        return EXIT_FAILURE;
    }
    else if (num_lines < 1)
    {
        fprintf(stdout, "ERROR: number not greater than 1\n");
        return EXIT_FAILURE;
    }
    else
    {
        *num_of_lines = num_lines;
        return EXIT_SUCCESS;
    }
}
/**
 * Check if the letter is valid in terms of ASCII
 * @param letter from the name
 * @return Success/Fail
 */
int check_char_name(char letter)
{
    if (letter <= MAX_LOWER_LETTER && letter >= MIN_LOWER_LETTER)
    {
        return EXIT_SUCCESS;
    }
    if (letter <= MAX_NAME_LINE_NUM && letter >= MIN_NAME_LINE_NUM)
    {
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

/**
 * Checks if a line name is valid in ASCII terms
 * @param name line name
 * @return Success/Fail
 */
int check_name_line(char* name)
{
    for (int i = 0; name[i] != '\0'; ++i)
    {
        if (check_char_name(name[i]) == EXIT_FAILURE)
        {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
/**
 * Check distance the line is a valid number
 * @param distance
 * @return Success/Fail
 */
int check_line_distance(int distance)
{
    if (distance <= MAX_LINE_DISTANCE && distance >= MIN_LINE_DISTANCE)
    {
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}
/**
 * Check duration the line is a valid number
 * @param duration
 * @return Success/Fail
 */
int check_line_duration(int duration)
{
    if (duration <= MAX_LINE_DURATION && duration >= MIN_LINE_DURATION)
    {
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}
/**
 * Checks if all line's info is valid
 * @param name
 * @param distance
 * @param duration
 * @return Success/fail
 */
int check_valid_line_info(char* name, int distance, int duration)
{
    if (check_name_line(name) == EXIT_FAILURE)
    {
        fprintf(stdout, "ERROR: bus name should include only "
                        "digits and small chars\n");
        return EXIT_FAILURE;

    }
    if (check_line_distance(distance) == EXIT_FAILURE)
    {
        fprintf(stdout, "ERROR: distance should be an int in"
                        " [0, 1000]\n");
        return EXIT_FAILURE;
    }
    if (check_line_duration(duration) == EXIT_FAILURE)
    {
        fprintf(stdout, "ERROR: duration should be an integer"
                        "in"
                        " [10,100]\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
/**
 * Builds a struct for the line
 * @param line pointer to the struct
 * @param name
 * @param distance
 * @param duration
 * @return pointer to the struct
 */
BusLine* build_line_struct(BusLine* line, const char* name, int distance,
                           int duration)
{
    if (line == NULL)
    {
        return NULL;
    }
    strcpy(line -> name, name);
    line -> distance = distance;
    line -> duration = duration;
    return line;
}

/**
 * checks user input regarding the line's name, distance and duration
 * @param flag 0 if he info is valid, 1 else
 * @param line_ptr pointer to the struct
 * @return Success/fail
 */
int ask_line_info(int *flag, BusLine* line_ptr)
{
    fprintf(stdout, "Enter line info. Then enter\n");
    char line_info_str[MAX_LINE_INFO];

    if (!fgets(line_info_str, MAX_LINE_INFO,stdin))
    {
        fprintf(stdout, "ERROR: fgets failed in line info.");
        *flag = EXIT_FAILURE;
        return EXIT_FAILURE;
    }
    char line_name[MAX_LINE_NAME];
    int line_distance = 0;
    int line_duration = 0;
    int line_info_scan = sscanf(line_info_str, "%49[^,],%d,%d",
                                line_name, &line_distance, &line_duration);
    if (line_info_scan != NUMBER__INFO_PARSED)
    {
        fprintf(stdout, "ERROR: sscanf failed in line info.");
        *flag = EXIT_FAILURE;
        return EXIT_FAILURE;
    }
    int is_info_valid = check_valid_line_info
            (line_name,line_distance, line_duration);
    if (is_info_valid == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    *flag = EXIT_SUCCESS;
    build_line_struct(line_ptr, line_name, line_distance,
                      line_duration);
    return EXIT_SUCCESS;
}
/**
 * Prints all the lines
 * @param lines_array pointer to the array of structs
 * @param array_size
 */
void print_lines_array(BusLine* lines_array, int array_size)
{
    for (int i = 0; i < array_size ;i++)
    {
        fprintf(stdout, "%s,%d,%d\n",
                lines_array[i].name, lines_array[i].distance,
                lines_array[i].duration);
    }
}
/**
 * Manages what happens when user entered "test"
 * @param lines_array
 * @param array_size
 * @param start of lines_array
 * @param end of lines_array
 * @return 1 for success, 0 else
 */
int test_sort_command(BusLine *lines_array, int array_size, BusLine* start,
                      BusLine* end)
{
   return is_test_passed(lines_array, array_size, start, end);
}
/**
 * Manages what happens when user entered "duration"
 * @param lines_array
 * @param array_size
 * @param start
 * @param end
 */
void duration_sort_command(BusLine *lines_array, int array_size,
                           BusLine* start, BusLine* end)
{
    quick_sort(start, end, DURATION);
    print_lines_array(lines_array, array_size);
}
/**
 * Manages what happens when user entered "distance"
 * @param lines_array
 * @param array_size
 * @param start
 * @param end
 */
void distance_sort_command(BusLine *lines_array, int array_size,
                           BusLine* start, BusLine* end)
{
    quick_sort(start, end, DISTANCE);
    print_lines_array(lines_array, array_size);

}
/**
 * Manages what happens when user entered "name"
 * @param lines_array
 * @param array_size
 * @param start
 * @param end
 */
void name_sort_command(BusLine *lines_array, int array_size,
                       BusLine* start, BusLine* end)
{
    bubble_sort(start, end);
    print_lines_array(lines_array, array_size);
}
/**
 * Manages all the possible commands
 * @param arg
 * @param lines_array
 * @param array_size
 * @return
 */
int command_management(char* arg, BusLine* lines_array, int array_size)
{
    BusLine* start = lines_array;
    BusLine* end = (array_size > 1) ? lines_array + array_size: lines_array;
    if (strcmp(arg, TEST) == 0)
    {
        return test_sort_command(lines_array,array_size, start, end);
    }
    else if (strcmp(arg, BY_DURATION) == 0)
    {
        duration_sort_command(lines_array,array_size, start, end);
    }
    else if (strcmp(arg, BY_DISTANCE) == 0)
    {
        distance_sort_command(lines_array, array_size, start, end);
    }
    else
    {
        name_sort_command(lines_array, array_size, start, end);
    }
    return EXIT_SUCCESS;
}
/**
 * Create the array of structs
 * @param arg test/distance/duration/name
 * @return Success/fail
 */
int create_lines_array(char* arg)
{
    int flag = EXIT_SUCCESS;
    int number_of_lines = 0;
    int* p_flag = &flag;
    int input_good = ask_num_of_lines(&number_of_lines);

    while (input_good == EXIT_FAILURE)
    {
        input_good = ask_num_of_lines(&number_of_lines);
    }

    BusLine* lines_array = malloc(number_of_lines * sizeof(BusLine));
    if (!lines_array)
    {
        return EXIT_FAILURE;
    }
    int corrected_lines = 0;
    while (corrected_lines < number_of_lines)
    {
        BusLine bus_line;
        int info =ask_line_info(p_flag, &bus_line);
        if (info == EXIT_FAILURE && flag == EXIT_FAILURE)
        {
            return EXIT_FAILURE;
        }
        if (info == EXIT_SUCCESS && flag == EXIT_SUCCESS)
            // If all went good flag should be 0
        {
            lines_array[corrected_lines] = bus_line;
            corrected_lines++;
        }
    }
    int command_m = command_management(arg, lines_array,
                                       number_of_lines);
    if (command_m == EXIT_FAILURE)
    {
        free(lines_array);
        lines_array = NULL;
    }
    free(lines_array);
    lines_array = NULL;
    return EXIT_SUCCESS;
}


/**
 * Control the first input of the user, which is argv[1[
 * @param argc
 * @param argv
 * @return
 */
int main (int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stdout, "USAGE: Enter one valid command.");
        return EXIT_FAILURE;
    }
    if (strcmp(argv[1], TEST) == 0
    || strcmp(argv[1], BY_DURATION) == 0
    || strcmp(argv[1], BY_DISTANCE) == 0
    || strcmp(argv[1], BY_NAME) == 0)
    {
        int creating_array =  create_lines_array(argv[1]);
        if (creating_array == EXIT_FAILURE)
        {
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }
    else
    {
        fprintf(stdout, "USAGE: Enter one valid command.");
        return EXIT_FAILURE;
    }
}

