#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <stdio.h>

using namespace std;

void always(string n_t, ifstream* in);
void one_bit(int size, ifstream* in);

int main(int argc, char** argv){
    //==========================================================================
    //check input
    // argv[1] is input
    // argv[2] is output
    if(argc < 3){
        fprintf(stderr, "Not enough arguments.\n");
        exit(1);
    }

    if(argc > 3){
        fprintf(stderr, "Too many arguments.\n");
        exit(1);
    }

    //==========================================================================
    //check if files exist
    ifstream input;
    ofstream output;
    output.open(argv[2]);
    input.open(argv[1]);

    if(!input || !output){
        fprintf(stderr, "File does not exist!\n");
        exit(1);
    }

    //==========================================================================
    //Always Taken and Non-Taken
    string T = "T";
    string NT = "NT";

    always(T, &input);

    input.clear();
    input.seekg(0);

    always(NT, &input);

    input.clear();
    input.seekg(0);

    //==========================================================================
    //Bimodal Predictor wiht a single bit of history

    one_bit(16, &input);

    //create N size table
    //mod the PC to get index of table and change the state of the value.
    // use an array to create a table





    return 0;
}

void one_bit(int size, ifstream* in){
    string line;
    char table[size];
    const char delim[2] = " ";
    char* token;
    int predicted_right = 0;
    int counter = 0;

    //initialize table to Taken (1).
    for (int i = 0; i < size; i++){
        table[i] = '1';
    }

    while(getline(*in, line) && counter != 25){
        char* new_line = (char*)line.c_str();
        token = strtok(new_line, delim);
        for(unsigned int i = 2; i < strlen(token); i++){
            token[i-2] = token[i];
        }
        int address = atoi(token);
        int index = address % size;
        char state = table[index];

        token = strtok(NULL, delim);

        if(strcmp(token, "NT") == 0){
            if(state == '0'){
                predicted_right++;
            } else {
                table[index] = '0';
            }
        } else if(strcmp(token, "T") == 0){
            if(state == '1'){
                predicted_right++;
            } else {
                table[index] = '1';
            }
        }
        counter++;
    }
    cout << "TABLE:" << endl;
    for(int i = 0; i < size; i++){
        cout << table[i] << " || ";
    }
    cout << endl;
    cout << predicted_right << "," << counter << ";" <<endl;
}

void always(string n_t, ifstream* in){
    string line;
    char* new_n_t = (char*) n_t.c_str();
    const char delim[2] = " ";
    char* token;
    int yerp = 0;
    int counter = 0;
    while(std::getline(*in,line)){
        char* new_line = (char*)line.c_str();
        token = strtok(new_line, delim);
        token = strtok(NULL, delim);
        counter++;
        if(strcmp(token, new_n_t) == 0){
            yerp++;
        }
    }
    //TODO: make it out put to file
    cout << yerp << "," << counter << ";" <<endl;
}
