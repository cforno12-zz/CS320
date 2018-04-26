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
void set_associative(std::string instruct, unsigned int address, int* counter, std::vector< std::vector<int> > &cache, int way, std::vector< std::deque<int> > &lru_queue, int num_slots, bool no_alloc);
void fully_associative(std::string instruct, unsigned int address, int& counter);
void next_line_prefetch(std::string instruct, unsigned int address, int &counter, std::vector< std::vector<int> > &cache, int way, std::vector< std::deque<int> > &lru_queue, int num_slots, bool miss);
void lru(int index, int tag, int way, std::vector < std::vector<int> > &cache, std::vector<std::deque<int> > &lru_queue, bool hit, int hit_way);
void hit_helper(bool &hit, int &hit_way, int tag, int &counter, int index, int way, std::vector< std::vector<int> > &cache);
#endif
