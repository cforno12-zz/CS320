#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>


//TODO: add argument for cache
int direct_mapped(std::string instruct, int address, int counter);
int set_associative(std::string instruct, int address, int counter);
int fully_associative(std::string instruct, int address, int counter);
int no_alloc(std::string instruct, int address, int counter);
int next_line_prefetch(std::string instruct, int address, int counter);
int prefetch_on_miss(std::string instruct, int address, int counter);
