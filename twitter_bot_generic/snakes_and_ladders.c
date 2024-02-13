#include <string.h> // For strlen(), strcmp(), etc
#include "markov_chain.h"

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))
#define EMPTY (-1)
#define BOARD_SIZE 100
#define MAX_GENERATION_LENGTH 60
#define DICE_MAX 6
#define NUM_OF_TRANSITIONS 20
#define ARGS 3
#define BASE 10
#define INDEX_OF_SEED 1
#define INDEX_OF_ROUTES 2
#define USAGE "Usage: Enter only between 3 or 4 parameters\n"


/**
 * represents the transitions by ladders and snakes in the game
 * each tuple (x,y) represents a ladder from x to if x<y or a snake otherwise
 */
const int transitions[][2] = {{13, 4},
                              {85, 17},
                              {95, 67},
                              {97, 58},
                              {66, 89},
                              {87, 31},
                              {57, 83},
                              {91, 25},
                              {28, 50},
                              {35, 11},
                              {8,  30},
                              {41, 62},
                              {81, 43},
                              {69, 32},
                              {20, 39},
                              {33, 70},
                              {79, 99},
                              {23, 76},
                              {15, 47},
                              {61, 14}};

/**
 * struct represents a Cell in the game board
 */
typedef struct Cell {
    int number; // Cell number 1-100
    int ladder_to;  // ladder_to represents the jump of the ladder in case
    // there is one from this square
    int snake_to;  // snake_to represents the jump of the snake in case there
    // is one from this square
    //both ladder_to and snake_to should be -1 if the Cell doesn't have them
} Cell;

/**
 * Prints a cell in the game
 * @param data will be a Cell struct
 */
static void print_cell(void * data);
/**
 * Compares cells in the game
 * @param first_data will be a Cell struct
 * @param second_data will be a Cell struct
 * @return
 */
static int compare_cell(void * first_data, void * second_data);
/**
 * Free one cell in the game
 * @param data will be a Cell struct
 */
static void free_cell(void * data);
/**
 * Copy one cell in the game
 * @param data will be a Cell struct
 * @return
 */
static void * copy_cell(void * data);
/**
 * Determine if a cell is the 100 cell in the game
 * @param data will be a Cell struct
 * @return
 */
static bool is_last_cell(void * data);

static void print_cell(void * data)
{
    Cell * cell = (Cell *) data;
    if (cell->ladder_to != -1)
    {
        printf("[%d]-ladder to %d ->", cell->number, cell->ladder_to);
    }
    else if (cell->snake_to != -1)
    {
        printf("[%d]-snake to %d ->", cell->number, cell->snake_to);
    }
    else
    {
        if (cell->number == BOARD_SIZE)
        {
            printf("[%d]", cell->number);
        }
        else
        {
            printf("[%d] ->", cell->number);
        }}
}

static int compare_cell(void * first_data, void * second_data)
{
    Cell * first_cell = (Cell *) first_data;
    Cell * second_cell = (Cell *) second_data;
    return first_cell->number - second_cell->number;
}
static void free_cell(void * data)
{
    Cell * cell = (Cell *) data;
    free(cell);
    cell = NULL;
}
static void * copy_cell(void * data)
{
    Cell * cell = (Cell *) data;
    Cell * copied_cell = calloc(1, sizeof(Cell));
    if (!copied_cell)
    {
        return NULL;
    }
    void * copy_cell = memcpy(copied_cell, cell, sizeof(Cell));
    return copy_cell;
}

static bool is_last_cell(void * data)
{
    Cell * cell = (Cell *) data;
    if (cell->number == BOARD_SIZE)
    {
        return true;
    }
    return false;
}



/** Error handler **/
static int handle_error(char *error_msg, MarkovChain **database)
{
    printf("%s", error_msg);
    if (database != NULL)
    {
        free_markov_chain(database);
    }
    return EXIT_FAILURE;
}


static int create_board(Cell *cells[BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        cells[i] = malloc(sizeof(Cell));
        if (cells[i] == NULL)
        {
            for (int j = 0; j < i; j++) {
                free(cells[j]);
            }
            handle_error(ALLOCATION_ERROR_MASSAGE,NULL);
            return EXIT_FAILURE;
        }
        *(cells[i]) = (Cell) {i + 1, EMPTY, EMPTY};
    }

    for (int i = 0; i < NUM_OF_TRANSITIONS; i++)
    {
        int from = transitions[i][0];
        int to = transitions[i][1];
        if (from < to)
        {
            cells[from - 1]->ladder_to = to;
        }
        else
        {
            cells[from - 1]->snake_to = to;
        }
    }
    return EXIT_SUCCESS;
}

/**
 * fills database
 * @param markov_chain
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int fill_database(MarkovChain *markov_chain)
{
    Cell* cells[BOARD_SIZE];
    if(create_board(cells) == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    MarkovNode *from_node = NULL, *to_node = NULL;
    size_t index_to;
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        add_to_database(markov_chain, cells[i]);
    }
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        from_node = get_node_from_database(markov_chain,
                                           cells[i])->data;
        if (cells[i]->snake_to != EMPTY || cells[i]->ladder_to != EMPTY)
        {
            index_to = MAX(cells[i]->snake_to,cells[i]->ladder_to) - 1;
            to_node = get_node_from_database(markov_chain,
                                             cells[index_to])
                    ->data;
            add_node_to_counter_list(from_node, to_node,
                                     markov_chain);
        }
        else
        {
            for (int j = 1; j <= DICE_MAX; j++)
            {
                index_to = ((Cell*) (from_node->data))->number + j - 1;
                if (index_to >= BOARD_SIZE)
                {
                    break;
                }
                to_node = get_node_from_database(markov_chain,
                                                 cells[index_to])
                        ->data;
                add_node_to_counter_list(from_node,
                                         to_node, markov_chain);
            }
        }
    }
    // free temp arr
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        free(cells[i]);
    }
    return EXIT_SUCCESS;
}
/**
 * Convert string to long and then to int
 * @param argument string
 * @return the int
 */
static int convert_str_to_int(char* argument)
{
    char *ptr;
    long longed = strtol(argument, &ptr, BASE);
    int integer = (int) longed;
    return integer;
}
/**
 * @param argc num of arguments
 * @param argv 1) Seed
 *             2) Number of routes to generate
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char *argv[])
{
    if (argc != ARGS)
    {
        printf(USAGE);
        return EXIT_FAILURE;
    }
    unsigned int seed_value = convert_str_to_int(argv[INDEX_OF_SEED]);
    srand(seed_value);
    int number_of_routes = convert_str_to_int(argv[INDEX_OF_ROUTES]);
    MarkovChain *markov_chain = calloc(1,
                                       sizeof(MarkovChain));
    if (!markov_chain)
    {
        fprintf(stdout, ALLOCATION_ERROR_MASSAGE);
        return EXIT_FAILURE;
    }
    *markov_chain = (MarkovChain) {NULL, print_cell,
                                   compare_cell,free_cell,
                                   copy_cell, is_last_cell};
    int is_database_filled = fill_database(markov_chain);
    if (is_database_filled == EXIT_FAILURE)
    {
        free_markov_chain(&markov_chain);
        fprintf(stdout, ALLOCATION_ERROR_MASSAGE);
        return EXIT_FAILURE;
    }
    MarkovNode *first_node;
    for (int i = 1; i <= number_of_routes; i++)
    {
        first_node = markov_chain->database->first->data;
        fprintf(stdout,"Random Walk %d: ", i);
        generate_random_sequence(
                markov_chain,first_node, MAX_GENERATION_LENGTH);
        fprintf(stdout, "\n");
    }
    free_markov_chain(&markov_chain);
    return EXIT_SUCCESS;
}
