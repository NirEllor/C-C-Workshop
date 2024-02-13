#include "cipher.h"
#include "tests.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define TEST "test"
#define NUM_ARGUMENTS_WORK 5
#define NUM_ARGUMENTS_TEST 2
#define ENCODE "encode"
#define DECODE "decode"
#define MAX_CHARS 1025
#define DECIMAL 10
#define MINUS '-'

/**
 * Runs all tests from tests.h when the user wants to
 * @return 0 if all good, 1 else
 */
int is_test_good()
{
    if ((test_encode_non_cyclic_lower_case_positive_k() ||
          test_encode_cyclic_lower_case_special_char_positive_k() ||
          test_encode_non_cyclic_lower_case_special_char_negative_k() ||
          test_encode_cyclic_lower_case_negative_k() ||
          test_encode_cyclic_upper_case_positive_k() ||
          test_decode_non_cyclic_lower_case_positive_k() ||
          test_decode_cyclic_lower_case_special_char_positive_k() ||
          test_decode_non_cyclic_lower_case_special_char_negative_k() ||
          test_decode_cyclic_lower_case_negative_k() ||
          test_decode_cyclic_upper_case_positive_k()))
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
/**
 * Checking if the user really typed 'test' and if so than tests
 * @param arg Second arg of argv[]
 * @return 0 if success, 1 otherwise
 */
int test_valid (char* arg)
{
    if (strcmp(arg,TEST) != 0)
    {
        fprintf(stderr,"Usage: cipher test\n");
        return EXIT_FAILURE;
    }
    return is_test_good();
}
/**
 * Checks if the command is encode/decode
 * @param arg - Second arg of argv[]
 * @return 0 if success, 1 otherwise
 */
int command_valid(char* arg)
{
    if (strcmp(arg, ENCODE) != 0 && strcmp(arg, DECODE) != 0)
    {
        fprintf(stderr,"The given command is invalid.\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;

}
/**
 * Checks if k is belong to Z group
 * @param arg - Third arg of argv[]
 * @return 0 if success, 1 otherwise
 */
int k_valid_input(const char* arg)
{
    for (int i = 0; arg[i] != '\0'; ++i)
    {
        if (isdigit((int)arg[i]) == 0 && !(arg[i] == MINUS && i == 0))
        {
            fprintf(stderr,"The given shift value is invalid.\n");
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
/**
 * Convert string k to int
  * @param arg - Third arg of argv[]
 * @return k in type int
 */
int k_long_to_int(char* arg)
{
    long int long_num = strtol(arg, NULL, DECIMAL);
    int int_num = (int)long_num;
    return int_num;
}
/**
 * Main function responsible of reading and writing to files
 * @param k
 * @param file_input
 * @param file_output
 * @param flag - 1 for encryption, 0 for decryption
 * @return 0 if success, 1 otherwise
 */
int file_handling(int k, char* file_input, char* file_output, int flag)
{
    FILE* f_input =  fopen(file_input, "r");
    if (!f_input)
    {
        fprintf(stderr,"The given file is invalid.\n");
        return EXIT_FAILURE;
    }
    FILE* f_output = fopen(file_output, "w");
    if (!f_output)
    {
        fclose(f_input);
        fprintf(stderr,"The given file is invalid.\n");
        return EXIT_FAILURE;
    }
    char line[MAX_CHARS] = {0};
    while (fgets(line, MAX_CHARS, f_input))
    {
        if (flag == 1)
        {
            encode(line, k);
            fputs(line, f_output);
        }
        else if (flag == 0)
        {
            decode(line, k);
            fputs(line, f_output);
        }
    }
    fclose(f_input);
    fclose(f_output);
    return EXIT_SUCCESS;
}
/**
 * Uses file_handling for encryption
 * @param k
 * @param file_input
 * @param file_output
 * @param flag 1
 * @return 0 if success, 1 otherwise
 */
int file_encode(int k, char* file_input, char* file_output, int flag)
{
    return file_handling(k, file_input, file_output, flag);
}
/**
 * Uses file_handling for decryption
 * @param k
 * @param file_input
 * @param file_output
 * @param flag 0
 * @return 0 if success, 1 otherwise
 */
int file_decode(int k, char* file_input, char* file_output, int flag)
{
    return file_handling(k, file_input, file_output, flag);
}
/**
 * Deciding whether it is encryption or decryption
 * @param arg1 - command, whether ENCODE or DECODE
 * @param k
 * @param file_input
 * @param file_output
 * @return 0 if success, 1 otherwise
 */
int file_working(char* arg1, int k, char* file_input, char* file_output)
{
    if (strcmp(arg1, ENCODE) == 0)
    {
        return file_encode(k, file_input, file_output, 1);
    }
    else
    {
        return file_decode(k, file_input, file_output, 0);
    }
}
/**
 * Responsible for preventing any crash made by the user input
 * @param arg1 command
 * @param arg2 k
 * @param arg3 file_input
 * @param arg4 file_output
 * @return 0 if success, 1 otherwise
 */
int work_valid(char* arg1, char* arg2, char* arg3, char* arg4)
{
    if (command_valid(arg1) == EXIT_FAILURE)
    {
       return EXIT_FAILURE;
    }
    if (k_valid_input(arg2) == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    int num_k = k_long_to_int(arg2);
    return file_working(arg1, num_k, arg3, arg4);
}
int main (int argc, char *argv[])
{
    if (argc == NUM_ARGUMENTS_TEST) // Test?
    {
        return test_valid(argv[1]);
    }
    else if (argc == NUM_ARGUMENTS_WORK) // Execute?
    {
        int worked = work_valid(argv[1], argv[2], argv[3],
                                argv[4]);
        if (worked == EXIT_SUCCESS)
        {
           return EXIT_SUCCESS;
        }
        return EXIT_FAILURE;
    }
    else // Not other option allowed
    {
        fprintf(stderr, "The program receives 1 or 4 arguments only.\n");
        return EXIT_FAILURE;
    }
}