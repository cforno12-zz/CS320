#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include "caches.hpp"

using namespace std;

int main(int argc, char** argv){

    //Error checking
    if(argc < 3){
        fprintf(stderr, "Not enough arguments.\n");
        exit(1);
    }

    if(argc > 3){
        fprintf(stderr, "Too many arguments.\n");
        exit(1);
    }

    ifstream input;
    ofstream output;
    output.open(argv[2]);
    input.open(argv[1]);

    if(!input || !output){
        fprintf(stderr, "File(s) does not exist\n");
        exit(1);
    }


    //==========================================================================
    //parsing variables
    string line;
    const char delim[2] = " ";
    char* buffer;

    int access_counter = 0;

    //==========================================================================
    //Direct mapping variables
    int size_of_1 = 0;
    int size_of_4 = 0;
    int size_of_16 = 0;
    int size_of_32 = 0;

    unsigned int direct_map_cache_1[1024]; //2^10 is 1K
    unsigned int direct_map_cache_4[4096]; //2^12 is 4K
    unsigned int direct_map_cache_16[16384]; //2^14 is 16K
    unsigned int direct_map_cache_32[32768]; //2^15 is 32K

    //==========================================================================
    //Set associative variables

    int set_counter_2 = 0;
    int set_counter_4 = 0;
    int set_counter_8 = 0;
    int set_counter_16 = 0;

    int cache_size_16K = 16 * 1024;
    int cache_line_size = 32;

    int rows_of_set_2 =(cache_size_16K)/(cache_line_size*2);
    int rows_of_set_4 = (cache_size_16K)/(cache_line_size*4);
    int rows_of_set_8 = (cache_size_16K)/(cache_line_size*8);
    int rows_of_set_16 = (cache_size_16K)/(cache_line_size*16);

    vector< vector<int> > set_cache_2(rows_of_set_2, vector<int> (2, -1));
    vector< vector<int> > set_cache_4(rows_of_set_4, vector<int> (4, -1));
    vector< vector<int> > set_cache_8(rows_of_set_8, vector<int> (8, -1));
    vector< vector<int> > set_cache_16(rows_of_set_16, vector<int> (16, -1));

    vector< deque<int> > lru_set_queue_2(rows_of_set_2);
    vector< deque<int> > lru_set_queue_4(rows_of_set_4);
    vector< deque<int> > lru_set_queue_8(rows_of_set_8);
    vector< deque<int> > lru_set_queue_16(rows_of_set_16);
    //==========================================================================
    //Fully Associative variables

    int fully_ways = (cache_size_16K)/(cache_line_size);

    int fully_counter = 0;

    vector< vector<int> > fully_cache(1, vector<int> (fully_ways, -1));

    vector< deque<int> > fully_lru(1);

    //===========================================================================
    //SET ASSOCIATIVE WITH NO ALLOCATION ON A MISS variables

    int set_counter_2_no_alloc = 0;
    int set_counter_4_no_alloc = 0;
    int set_counter_8_no_alloc = 0;
    int set_counter_16_no_alloc = 0;

    //ROWS SAME AS SET ASSOCIATIVE
    vector< vector<int> > set_cache_2_no_alloc(rows_of_set_2, vector<int> (2, -1));
    vector< vector<int> > set_cache_4_no_alloc(rows_of_set_4, vector<int> (4, -1));
    vector< vector<int> > set_cache_8_no_alloc(rows_of_set_8, vector<int> (8, -1));
    vector< vector<int> > set_cache_16_no_alloc(rows_of_set_16, vector<int> (16, -1));

    vector< deque<int> > lru_set_queue_2_no_alloc(rows_of_set_2);
    vector< deque<int> > lru_set_queue_4_no_alloc(rows_of_set_4);
    vector< deque<int> > lru_set_queue_8_no_alloc(rows_of_set_8);
    vector< deque<int> > lru_set_queue_16_no_alloc(rows_of_set_16);
    //==========================================================================
    //PREFETCH ALWAYS
    int prefetch_always_2 = 0;
    int prefetch_always_4 = 0;
    int prefetch_always_8 = 0;
    int prefetch_always_16 = 0;

    //ROWS SAME AS SET ASSOCIATIVE
    vector< vector<int> > prefetch_always_cache_2(rows_of_set_2, vector<int> (2, -1));
    vector< vector<int> > prefetch_always_cache_4(rows_of_set_4, vector<int> (4, -1));
    vector< vector<int> > prefetch_always_cache_8(rows_of_set_8, vector<int> (8, -1));
    vector< vector<int> > prefetch_always_cache_16(rows_of_set_16, vector<int> (16, -1));

    vector< deque<int> > prefetch_always_lru_queue_2(rows_of_set_2);
    vector< deque<int> > prefetch_always_lru_queue_4(rows_of_set_4);
    vector< deque<int> > prefetch_always_lru_queue_8(rows_of_set_8);
    vector< deque<int> > prefetch_always_lru_queue_16(rows_of_set_16);
    //==========================================================================
    //PREFETCH ON MISS
    int prefetch_always_2_miss = 0;
    int prefetch_always_4_miss = 0;
    int prefetch_always_8_miss = 0;
    int prefetch_always_16_miss = 0;

    //ROWS SAME AS SET ASSOCIATIVE
    vector< vector<int> > prefetch_always_cache_2_miss(rows_of_set_2, vector<int> (2, -1));
    vector< vector<int> > prefetch_always_cache_4_miss(rows_of_set_4, vector<int> (4, -1));
    vector< vector<int> > prefetch_always_cache_8_miss(rows_of_set_8, vector<int> (8, -1));
    vector< vector<int> > prefetch_always_cache_16_miss(rows_of_set_16, vector<int> (16, -1));

    vector< deque<int> > prefetch_always_lru_queue_2_miss(rows_of_set_2);
    vector< deque<int> > prefetch_always_lru_queue_4_miss(rows_of_set_4);
    vector< deque<int> > prefetch_always_lru_queue_8_miss(rows_of_set_8);
    vector< deque<int> > prefetch_always_lru_queue_16_miss(rows_of_set_16);

    //==========================================================================
    //begin parsing file
    while(getline(input, line) ){
        char* new_line = (char*) line.c_str();
        buffer = strtok(new_line, delim);
        string instruction(buffer);
        buffer = strtok(NULL, delim);
        memmove (buffer, buffer+2, strlen (buffer+2) + 2);
        stringstream buffer02;
        unsigned int address;
        buffer02 << hex << buffer;
        buffer02 >> address;

        //DIRECT MAPPING
        direct_mapped(instruction, address, 1, direct_map_cache_1, &size_of_1);
        direct_mapped(instruction, address, 4, direct_map_cache_4, &size_of_4);
        direct_mapped(instruction, address, 16, direct_map_cache_16, &size_of_16);
        direct_mapped(instruction, address, 32, direct_map_cache_32, &size_of_32);
        //SET ASSOCIATIVE
        set_associative(instruction, address, &set_counter_2, set_cache_2, 2, lru_set_queue_2, rows_of_set_2, false);
        set_associative(instruction, address, &set_counter_4, set_cache_4, 4, lru_set_queue_4, rows_of_set_4, false);
        set_associative(instruction, address, &set_counter_8, set_cache_8, 8, lru_set_queue_8, rows_of_set_8, false);
        set_associative(instruction, address, &set_counter_16, set_cache_16, 16, lru_set_queue_16, rows_of_set_16, false);
        //FULLY ASSOCIATIVE
        fully_associative(instruction, address, fully_counter, fully_cache, fully_lru, fully_ways);
        //SET ASSOCIATIVE WITH NO ALLOCATION ON A MISS
        set_associative(instruction, address, &set_counter_2_no_alloc, set_cache_2_no_alloc, 2, lru_set_queue_2_no_alloc, rows_of_set_2, true);
        set_associative(instruction, address, &set_counter_4_no_alloc, set_cache_4_no_alloc, 4, lru_set_queue_4_no_alloc, rows_of_set_4, true);
        set_associative(instruction, address, &set_counter_8_no_alloc, set_cache_8_no_alloc, 8, lru_set_queue_8_no_alloc, rows_of_set_8, true);
        set_associative(instruction, address, &set_counter_16_no_alloc, set_cache_16_no_alloc, 16, lru_set_queue_16_no_alloc, rows_of_set_16, true);
        //PREFETCH ALWAYS
        next_line_prefetch(instruction, address, prefetch_always_2, prefetch_always_cache_2, 2, prefetch_always_lru_queue_2, rows_of_set_2, false);
        next_line_prefetch(instruction, address, prefetch_always_4, prefetch_always_cache_4, 4, prefetch_always_lru_queue_4, rows_of_set_4, false);
        next_line_prefetch(instruction, address, prefetch_always_8, prefetch_always_cache_8, 8, prefetch_always_lru_queue_8, rows_of_set_8, false);
        next_line_prefetch(instruction, address, prefetch_always_16, prefetch_always_cache_16, 16, prefetch_always_lru_queue_16, rows_of_set_16, false);
        //PREFETCH ON MISS
        next_line_prefetch(instruction, address, prefetch_always_2_miss, prefetch_always_cache_2_miss, 2, prefetch_always_lru_queue_2_miss, rows_of_set_2, true);
        next_line_prefetch(instruction, address, prefetch_always_4_miss, prefetch_always_cache_4_miss, 4, prefetch_always_lru_queue_4_miss, rows_of_set_4, true);
        next_line_prefetch(instruction, address, prefetch_always_8_miss, prefetch_always_cache_8_miss, 8, prefetch_always_lru_queue_8_miss, rows_of_set_8, true);
        next_line_prefetch(instruction, address, prefetch_always_16_miss, prefetch_always_cache_16_miss, 16, prefetch_always_lru_queue_16_miss, rows_of_set_16, true);

        access_counter++;
    }

    //==========================================================================
    //Printing results to output file

    //printing out for direct mapping
    output << size_of_1 << "," << access_counter << "; ";
    output << size_of_4 << "," << access_counter << "; ";
    output << size_of_16 << "," << access_counter << "; ";
    output << size_of_32 << "," << access_counter << ";" << endl;

    //printing for set associative
    output << set_counter_2 << "," << access_counter << "; ";
    output << set_counter_4 << "," << access_counter << "; ";
    output << set_counter_8 << "," <<access_counter << "; ";
    output << set_counter_16 << "," <<access_counter << "; " << endl;

    //printing set associative no alloc
    output << fully_counter << ", " << access_counter <<endl;
    output << "BLANK"<<endl;

    //NO ALLOC
    output << set_counter_2_no_alloc << "," << access_counter << "; ";
    output << set_counter_4_no_alloc << "," << access_counter << "; ";
    output << set_counter_8_no_alloc << "," << access_counter << "; ";
    output << set_counter_16_no_alloc << "," << access_counter << "; " << endl;

    //ALWAYS PREFETCH
    output << prefetch_always_2 << "," << access_counter << "; ";
    output << prefetch_always_4 << "," << access_counter << "; ";
    output << prefetch_always_8 << "," << access_counter << "; ";
    output << prefetch_always_16 << "," << access_counter << "; " << endl;

    //PREFETCH ON MISS
    output << prefetch_always_2_miss << "," << access_counter << "; ";
    output << prefetch_always_4_miss << "," << access_counter << "; ";
    output << prefetch_always_8_miss << "," << access_counter << "; ";
    output << prefetch_always_16_miss << "," << access_counter << "; " << endl;
}
