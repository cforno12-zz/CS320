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

    int rows_of_set_2 = (cache_size_16K/cache_line_size)/2;
    int rows_of_set_4 = (cache_size_16K/cache_line_size)/4;
    int rows_of_set_8 = (cache_size_16K/cache_line_size)/8;
    int rows_of_set_16 = (cache_size_16K/cache_line_size)/16;

    unsigned int** set_cache_2 = new unsigned int*[rows_of_set_2];
    for(int i = 0; i < rows_of_set_2; i++){
        set_cache_2[i] = new unsigned int[2];
    }

    unsigned int** set_cache_4 = new unsigned int*[rows_of_set_4];
    for(int i = 0; i < rows_of_set_4; i++){
        set_cache_4[i] = new unsigned int[4];
    }

    unsigned int** set_cache_8 = new unsigned int*[rows_of_set_8];
    for(int i = 0; i < rows_of_set_8; i++){
        set_cache_8[i] = new unsigned int[8];
    }
    unsigned int** set_cache_16 = new unsigned int*[rows_of_set_16];
    for(int i = 0; i < rows_of_set_16; i++){
        set_cache_16[i] = new unsigned int[16];
    }

    deque<unsigned int> lru_set_queue_2;
    deque<unsigned int> lru_set_queue_4;
    deque<unsigned int> lru_set_queue_8;
    deque<unsigned int> lru_set_queue_16;
    //==========================================================================
    //Fully Associative variables

    //int fully_counter_


    //==========================================================================
    //begin parsing file
    while(getline(input, line)){
        char* new_line = (char*) line.c_str();
        buffer = strtok(new_line, delim);
        string instruction(buffer);
        buffer = strtok(NULL, delim);
        memmove (buffer, buffer+2, strlen (buffer+2) + 2);
        stringstream buffer02;
        unsigned int address;
        buffer02 << hex << buffer;
        buffer02 >> address;

        //INSTRCTION is instruction variable
        //ADDRESS is address variable

        //DIRECT MAPPING
        direct_mapped(instruction, address, 1, direct_map_cache_1, &size_of_1);
        direct_mapped(instruction, address, 4, direct_map_cache_4, &size_of_4);
        direct_mapped(instruction, address, 16, direct_map_cache_16, &size_of_16);
        direct_mapped(instruction, address, 32, direct_map_cache_32, &size_of_32);
        //SET ASSOCIATIVE
        set_associative(instruction, address, &set_counter_2, set_cache_2, 2, lru_set_queue_2);
        set_associative(instruction, address, &set_counter_4, set_cache_4, 4, lru_set_queue_4);
        set_associative(instruction, address, &set_counter_8, set_cache_8, 8, lru_set_queue_8);
        set_associative(instruction, address, &set_counter_16, set_cache_16, 16, lru_set_queue_16);
        //FULLY ASSOCIATIVE



        access_counter++;
    }

    //==========================================================================
    //Printing results to output file

    output << size_of_1 << "," << access_counter << "; ";
    output << size_of_4 << "," << access_counter << "; ";
    output << size_of_16 << "," << access_counter << "; ";
    output << size_of_32 << "," << access_counter << ";" << endl;
}
