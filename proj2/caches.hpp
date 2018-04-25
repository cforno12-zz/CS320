#ifndef CACHE_H
#define CACHE_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdbool.h>
#include <math.h>
#include <vector>
#include <deque>

void direct_mapped(std::string instruct, unsigned int address, int cache_size, unsigned int cache[], int* counter);
void set_associative(std::string instruct, unsigned int address, int* counter, std::vector< std::vector<int> > &cache, int way, std::vector< std::deque<int> > &lru_queue, int num_slots);
//void lru_helper(std::deque<unsigned int> lru_queue);
int fully_associative(std::string instruct, unsigned int address, int counter);
int no_alloc(std::string instruct, unsigned int address, int counter);
int next_line_prefetch(std::string instruct, unsigned int address, int counter);
int prefetch_on_miss(std::string instruct, unsigned int address, int counter);
#endif
