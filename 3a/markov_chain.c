#include "markov_chain.h"
#include <string.h>
#define POINT '.'
Node* add_to_database(MarkovChain *markov_chain, char *data_ptr)
{

    // Now checking if nothing is in the DB
    if (markov_chain->database == NULL)
    {
        // Allocating memory for linked list
        markov_chain->database = calloc(1,
                                    sizeof(LinkedList));
        if (markov_chain->database == NULL)
        {
            return NULL;
        }
    }

    // Now checking if the string already sits in the DB
    Node *is_node_in_db = get_node_from_database(markov_chain, data_ptr);
    if (is_node_in_db)
    {
        return is_node_in_db;
    }
    // We haven't inserted the data to the db yet, so we will allocate memory
    // in
    // the db for new markov node containing pointer to the data
    MarkovNode *new_markov_node = calloc(1,
                                         sizeof(MarkovNode));
    if (!new_markov_node)
    {
        return NULL;
    }
    new_markov_node->data = calloc(1,
                                   strlen(data_ptr) + 1);
    if (!new_markov_node->data)
    {
        return NULL;
    }
    strcpy((new_markov_node->data), data_ptr);
    new_markov_node->frequencies_list = NULL;
    new_markov_node->freq_sum = 0;
    new_markov_node->cur_freq_size = 0;
    new_markov_node->freq_list_size = 1;
    int added = add(markov_chain->database, new_markov_node);
    if (added == EXIT_FAILURE)
    {
        return NULL;
    }
    return markov_chain->database->last;
}

Node* get_node_from_database(MarkovChain *markov_chain, char *data_ptr)
{
    Node *first_node =  markov_chain->database->first;
    int size_of_db = markov_chain->database->size;
    for (int i = 0; i < size_of_db; ++i)
    {
        if (strcmp(first_node->data->data, data_ptr) == 0) // Found
        {
            return first_node;
        }
        first_node = first_node->next;
    }
    return NULL; // Not found
}

bool add_node_to_counter_list(MarkovNode *first_node, MarkovNode *second_node)
{
    first_node->freq_sum++;
    if (first_node->frequencies_list == NULL)
    {
        first_node->frequencies_list = calloc(
                first_node->freq_list_size,
                sizeof(MarkovNodeFrequency));
        if (!first_node->frequencies_list)
        {
            return false;
        }

        first_node->frequencies_list[first_node->cur_freq_size] =
                (MarkovNodeFrequency) {second_node, 1};
        first_node->cur_freq_size++;
        return true;
    }
    if (first_node->freq_list_size == first_node->cur_freq_size)
    {
        first_node->freq_list_size++;
        MarkovNodeFrequency *updated_freq_list =realloc
                (first_node->frequencies_list,
                 first_node->freq_list_size *
                 sizeof(MarkovNodeFrequency));
        if (!updated_freq_list)
        {
            return false;
        }
        first_node->frequencies_list = updated_freq_list;
    }
    for (int i = 0; i < first_node->cur_freq_size; ++i)
    {
        if (strcmp(first_node->frequencies_list[i].markov_node->data,
                   (second_node->data)) == 0)
        {
            first_node->frequencies_list[i].frequency++;
            return true;
        }
    }
    (first_node->frequencies_list)[first_node->cur_freq_size] =
            (MarkovNodeFrequency) {second_node, 1};
    first_node->cur_freq_size++;
    return true;
}

void free_markov_chain(MarkovChain ** ptr_chain)
{
    int db_size = (*ptr_chain)->database->size;
    Node *first_node = (*ptr_chain)->database->first;
    Node *next_node = NULL;

    for (int i = 0; i < db_size; ++i)
    {
        next_node = first_node->next;
        free(first_node->data->data); // Free string
        free(first_node->data->frequencies_list); // Free freq_list
        free(first_node->data); // Free Markov_Node
        free(first_node); // Free Node
        first_node = next_node; // Move to next Node in DB
    }
    free((*ptr_chain)->database); // Free DB
    free(*ptr_chain); // Free chain
    *ptr_chain = NULL;
}

int is_final_word(char * word)
{
    if (word[strlen(word)- 1] == POINT)
    {
        return 1;
    }
    return 0;
}

int get_random_number(int max_number)
{
    return rand() % max_number;

}

Node* get_node_index(MarkovChain *markov_chain, int index)
{
    Node * first_node = markov_chain->database->first;
    for (int i = 0; i < index; ++i)
    {
        first_node = first_node->next;
    }
    return first_node;
}

MarkovNode* get_first_random_node(MarkovChain *markov_chain)
{
    Node* chosen_node;
    int number_random;
    do
    {
        number_random = get_random_number(
                markov_chain->database->size);
        chosen_node = get_node_index(markov_chain, number_random);
    } while (is_final_word(chosen_node->data->data) == 1);
    return chosen_node->data;

}

MarkovNode* get_next_random_node(MarkovNode *state_struct_ptr)
{
    MarkovNodeFrequency *freq_list_cur_elem =
            (state_struct_ptr->frequencies_list);
    unsigned int freq_size = state_struct_ptr->cur_freq_size;
    int number_random = get_random_number(state_struct_ptr->freq_sum);
    for (unsigned int i = 0; i < freq_size; i++)
    {
        for (unsigned int j = 0; j < freq_list_cur_elem[i].frequency; j++)
        {
            if (number_random == 0)
            {
                return freq_list_cur_elem[i].markov_node;
            }
            number_random--;
        }
    }
    return NULL; // Won't happen
}

void generate_random_sequence(MarkovChain *markov_chain, MarkovNode *
first_node, int max_length)
{
    MarkovNode *next_node = first_node;
    printf("%s", next_node->data);
    int counter = 1;
    while ((counter < max_length) && (counter < markov_chain->database->size))
    {
        fprintf(stdout, " ");
        next_node = get_next_random_node(next_node);
        fprintf(stdout, "%s", next_node->data);
        if (is_final_word(next_node->data) == 1)
        {
            break;
        }
        counter++;
    }
}
