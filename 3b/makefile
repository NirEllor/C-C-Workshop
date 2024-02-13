CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

#all: tweets snake

tweets: markov_chain.c markov_chain.h tweets_generator.c linked_list.c linked_list.h
	$(CC) $(CFLAGS) -o $@ $^

snake: markov_chain.c markov_chain.h snakes_and_ladders.c linked_list.c linked_list.h
	$(CC) $(CFLAGS) -o $@ $^