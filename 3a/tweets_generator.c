#include "markov_chain.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#define MAX_ARGS 5
#define MIN_ARGS 4
#define BASE 10
#define INDEX_OF_SEED 1
#define INDEX_OF_TWEETS_NUM 2
#define INDEX_OF_PATH 3
#define INDEX_OF_WORDS_NUM 4
#define NEWLINE '\n'
#define MAX_WORDS_IN_SENTENCE 1002
#define MAX_TWEET_LENGTH 20
#define INVALID_PATH "Error: Invalid path\n"
#define USAGE "Usage: Enter only between 3 or 4 parameters\n"

/**
 *
 * @param path
 * @return 1 if valid, 0 else
 */
int check_valid_path(char *path)
{
    FILE *tweets_path = fopen(path, "r");
    if (!tweets_path)
    {
        return 0;
    }
    fclose(tweets_path);
    return 1;
}
/**
 *
 * @param argument
 * @return string in int
 */
int convert_str_to_int(char* argument)
{
    char *ptr;
    long longed = strtol(argument, &ptr, BASE);
    int integer = (int) longed;
    return integer;
}
/**
 *
 * @param file_path
 * @return number of rows
 */
int count_rows_in_file(char *file_path)
{

    FILE *tweets_path = fopen(file_path, "r");
    if (!tweets_path)
    {
        fprintf(stdout, INVALID_PATH);
        return 0;
    }
    int num_of_words = 0;
    int c;
    int continue_counting = 1;
    int *ptr_continue_counting = &continue_counting;
    do
    {
        c = fgetc(tweets_path);
        if ((isspace(c) || c == NEWLINE) && continue_counting == 1)
        {
            num_of_words++;
            *ptr_continue_counting = 0;
        }
        else if ((!isspace(c) && c != NEWLINE) && continue_counting == 0)
        {
            num_of_words++;
            *ptr_continue_counting = 1;
        }
    } while (c != EOF);
    fclose(tweets_path);
    return num_of_words;

}


/**
 *
 * @param fp path to file
 * @param words_to_read
 * @param markov_chain pointer
 * @return 0 for success, NULL otherwise
 */
int fill_database(FILE *fp, int words_to_read, MarkovChain *markov_chain)
{

    char input[MAX_WORDS_IN_SENTENCE];

    Node *cur_node = NULL, *prev_node = NULL;
    while (fgets(input, MAX_WORDS_IN_SENTENCE,
                 fp) != NULL && (words_to_read > 0))
    {
        char *cur_word = strtok(input, " \n");
        while (cur_word != NULL)
        {
            cur_node = add_to_database(markov_chain, cur_word);
            if (!cur_node)
            {
                free_markov_chain(&markov_chain);
                return EXIT_FAILURE;
            }
            if (prev_node != NULL)
            {
                bool added = add_node_to_counter_list(prev_node->data,
                                                      cur_node->data
                                                      );
                if (!added)
                {
                    free_markov_chain(&markov_chain);
                    return EXIT_FAILURE;
                }

            }
            words_to_read--;
            if (words_to_read == 0)
            {
                break;
            }
            prev_node = cur_node;
            cur_word = strtok(NULL, " \n");
        }
    }
    return EXIT_SUCCESS;
}
/**
 *
 * @param num_args argc in main
 * @param tweets_num
 * @param path
 * @param words_num
 * @param num_rows_to_read
 * @param num_of_tweets
 * @return 1 if valid, 0 else
 */
int check_valid_input(int num_args, char* tweets_num,
                      char* path, char* words_num, int* num_rows_to_read,
                      int* num_of_tweets)
{
    if (num_args > MAX_ARGS || num_args < MIN_ARGS)
    {
        fprintf(stdout, USAGE);
        return EXIT_FAILURE;
    }
    if (!check_valid_path(path))
    {
        fprintf(stdout, INVALID_PATH);
        return EXIT_FAILURE;

    }

    int nums_rows_actual = count_rows_in_file(
            path);
    if (num_args == MAX_ARGS)
    {
        *num_rows_to_read = convert_str_to_int(
                words_num);
        if (*num_rows_to_read > nums_rows_actual)
        {
            *num_rows_to_read = nums_rows_actual;
        }

    }
    else
    {
        *num_rows_to_read = nums_rows_actual;
    }
    *num_of_tweets = convert_str_to_int(
            tweets_num);
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    int num_rows_to_read,num_of_tweets;
    int is_valid = check_valid_input(
            argc,argv[INDEX_OF_TWEETS_NUM],
            argv[INDEX_OF_PATH],argv[INDEX_OF_WORDS_NUM],
                      &num_rows_to_read, &num_of_tweets);
    if (is_valid == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    int seed_value = convert_str_to_int(argv[INDEX_OF_SEED]);
    srand(seed_value);
    FILE * open_text_file = fopen(argv[INDEX_OF_PATH], "r");
    if (!open_text_file)
    {
        fprintf(stdout, INVALID_PATH);
        return EXIT_FAILURE;
    }
    MarkovChain *markov_chain = calloc(1,
                                       sizeof(MarkovChain));
    if (!markov_chain)
    {
        fprintf(stdout, ALLOCATION_ERROR_MASSAGE);
        fclose(open_text_file);
        return EXIT_FAILURE;
    }
    int is_database_filled = fill_database(
            open_text_file,num_rows_to_read,markov_chain);
    if (is_database_filled == EXIT_FAILURE)
    {
        free_markov_chain(&markov_chain);
        fprintf(stdout, ALLOCATION_ERROR_MASSAGE);
        fclose(open_text_file);
        return EXIT_FAILURE;
    }
    MarkovNode *first_node;
    for (int i = 1; i <= num_of_tweets; i++)
    {
        first_node = get_first_random_node(markov_chain);
        fprintf(stdout,"Tweet %d: ", i);
        generate_random_sequence(
                markov_chain,first_node, MAX_TWEET_LENGTH);
        fprintf(stdout, "\n");
    }
    fclose(open_text_file);
    free_markov_chain(&markov_chain);
    return EXIT_SUCCESS;
}