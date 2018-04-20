#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sstream>

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

    while(getline(input, line) && access_counter != 25){
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

        



        access_counter++;
    }
}
