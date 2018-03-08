#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <stdio.h>

using namespace std;

void always(string n_t, ifstream* in);
void one_bit(int size, ifstream* in);
int hexadecimalToDecimal(char hexVal[]);

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
        fprintf(stderr, "File(s) does not exist!\n");
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

    // input.clear();
    // input.seekg(0);

    // one_bit(32, &input);

    // input.clear();
    // input.seekg(0);

    // one_bit(128, &input);

    // input.clear();
    // input.seekg(0);

    // one_bit(256, &input);

    // input.clear();
    // input.seekg(0);

    // one_bit(512, &input);

    // input.clear();
    // input.seekg(0);

    // one_bit(1024, &input);

    // input.clear();
    // input.seekg(0);


    // one_bit(2048, &input);

    // input.clear();
    // input.seekg(0);

    cout << endl;

    //==========================================================================
    //Bimodal Predictor with 2-bit saturating counters

    return 0;
}

void one_bit(int size, ifstream* in){
    string line;
    int table[size];
    const char delim[2] = " ";
    char* token;
    int predicted_right = 0;
    int counter = 0;

    //initialize table to Taken (1).
    for (int i = 0; i < size; i++){
        table[i] = 1;
    }

    while(getline(*in, line)){
        char* new_line = (char*)line.c_str();
        token = strtok(new_line, delim);
        int address = hexadecimalToDecimal(token);
        int index = address % size;
        int state = table[index];

        token = strtok(NULL, delim);

        if(strcmp(token, "NT") == 0){
            if(state == 0){
                predicted_right++;
            } else {
                table[index] = 0;
            }
        } else if(strcmp(token, "T") == 0){
            if(state == 1){
                predicted_right++;
            } else {
                table[index] = 1;
            }
        }
        counter++;
    }
    // cout << endl;
    // cout << "TABLE:" << endl;
    // for(int i = 0; i < size; i++){
    //     cout << "| " << i << "="<< table[i] << " |";
    // }
    // cout << endl;
    cout << predicted_right << "," << counter << "; ";
}

void always(string n_t, ifstream* in){
    string line;
    char* new_n_t = (char*) n_t.c_str();
    const char delim[2] = " ";
    char* token;
    int predicted_right = 0;
    int counter = 0;
    while(std::getline(*in,line)){
        char* new_line = (char*)line.c_str();
        token = strtok(new_line, delim);
        token = strtok(NULL, delim);
        counter++;
        if(strcmp(token, new_n_t) == 0){
            predicted_right++;
        }
    }
    //TODO: make it out put to file
    cout << predicted_right << "," << counter << ";" <<endl;
}


//retrieved function from:
// https://www.geeksforgeeks.org/program-hexadecimal-decimal/
int hexadecimalToDecimal(char hexVal[]) {
    for(unsigned int i = 2; i < strlen(hexVal); i++){
        hexVal[i-2] = hexVal[i];
    }
    hexVal[strlen(hexVal) - 2] = '\0';
    int len = strlen(hexVal);
    // Initializing base value to 1, i.e 16^0
    int base = 1;
    int dec_val = 0;

    // Extracting characters as digits from last character
    for (int i=len-1; i>=0; i--) {
            // if character lies in '0'-'9', converting
            // it to integral 0-9 by subtracting 48 from
            // ASCII value.
            if (hexVal[i]>='0' && hexVal[i]<='9') {
                    dec_val += (hexVal[i] - 48)*base;
                    // incrementing base by power
                    base = base * 16;
                }
            // if character lies in 'A'-'F' , converting
            // it to integral 10 - 15 by subtracting 55
            // from ASCII value
            else if (hexVal[i]>='a' && hexVal[i]<='f') {
                    dec_val += (hexVal[i] - 87)*base;
                    // incrementing base by power
                    base = base*16;
                }
        }
    return dec_val;
}
