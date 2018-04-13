#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <vector>
#include <math.h>
#include <algorithm>
#include <iostream>
#include "branches.h"

using namespace std;

void perceptron(ifstream* in, ofstream* out, int table_size, int gh_length){
    //output counters
    int counter = 0;
    int predicted_right = 0;

    //table size is 512
    //gh_length is 11

    //perceptron stuff
    vector<vector<int> > perceptron_table;
    vector<int> global_history;
    vector<int> weights;
    /* bit x_0 is always set to 1, providing a bias.*/
    int threshold = 1.93*gh_length + 14;
    int y = 0;

    string taken;

    int t;
    int bias[table_size];
    bias[0] = 1;
    for(int i = 1; i < table_size; i++){
        bias[i] = -1;
    }

    for(int i = 0; i < gh_length; i++){
        weights.push_back(0);
    }

    global_history.push_back(1);
    for(int i = 1; i < gh_length; i++){
        global_history.push_back(-1);
    }

    for(int i = 0; i < table_size; i++){
        perceptron_table.push_back(weights);
    }

    //output (y) is a dot product of the weights and the input vector.
    /* y = w_0 + \sum_{i=1}^{n} x_i * w_i */
    // x_i is -1 (NT) or 1 (T)

    //parsing stuff
    string line;
    const char delim[2] = " ";
    char* token;

    while(getline(*in, line)){
        char* new_line = (char*) line.c_str();
        token = strtok(new_line, delim);
        unsigned int address = hexadecimalToDecimal(token);
        int index = address % table_size;
        vector<int> perceptron = perceptron_table[index];
        y = bias[index];

        token = strtok(NULL, delim);
        if(strcmp(token, "NT") == 0){
            t = -1;
        } else if(strcmp(token, "T") == 0){
            t = 1;
        } else {
            fprintf(stderr, "Something wrong with perceptron predictor.\n");
            exit(1);
            t = 0;
        }


        /*y is our prediction*/
        /* Calculating output*/
        for(int i = 1; i < gh_length; i++) {
            y = y + (global_history[i]*perceptron[i]);
        }

        /*checking if our predicton is correct*/
        if((y >= 0 && (strcmp(token, "T")==0)) || (y < 0 && (strcmp(token, "NT") == 0))){
            predicted_right++;
        }

        /*updating global history bit*/
        if(((y > 0 && t == -1) || (y < 0 && t == 1)) || (abs(y) <= threshold)){
            bias[index] += t;

            for(int i = 1; i < gh_length; i++){
                if(t == global_history[i]){
                    perceptron[i] = perceptron[i] + 1;
                } else {
                    perceptron[i] = perceptron[i] - 1;
                }
            }
        }

        //shifting global history register to input the branch at the least significant bit
        std::rotate(global_history.begin(), global_history.begin()+global_history.size()-1, global_history.end());
        global_history[0] = t;

        counter++;
    }
    *out << predicted_right << "," << counter << ";" << endl;
}

void tournament(ifstream* in, ofstream* out){

    //tournament stuff
    int selector_table[2048];

    //gshare stuff
    unsigned int history = 0;
    unsigned int history_mask = 0b11111111111; //11-bit mask
    int gshare_table[2048];


    //bimodal stuff
    int bimodal_table[2048];

    //parsing stuff
    string line;
    const char delim[2] = " ";
    char* token;

    //global counters
    int predicted_right = 0;
    int gshare_right = 0;
    int bimodal_right = 0;
    int total_counter = 0;

    //TODO: initialize tables DONE
    for(int i = 0; i < 2048; i++){
        selector_table[i] = 0;
        bimodal_table[i] = 3;
        gshare_table[i] = 3;
    }


    while(getline(*in, line)){
        char* new_line = (char*) line.c_str();
        token = strtok(new_line, delim);
        unsigned int address = hexadecimalToDecimal(token);
        unsigned int new_history = history & history_mask;
        int gshare_index = (address % 2048) ^ new_history;
        int selector_and_bimodal_index = address % 2048;
        int state = selector_table[selector_and_bimodal_index];

        token = strtok(NULL, delim);

        switch(state){
            case 0:
                //Strong Gshare
                gshare_helper(gshare_table, token, gshare_index, &history, &gshare_right);
                two_bit_helper(bimodal_table, token, &bimodal_right, selector_and_bimodal_index);

                if(gshare_right == 1 && bimodal_right == 1){
                    //both predictors are correct
                    predicted_right++;
                } else if(gshare_right == 1 && bimodal_right == 0){
                    //only gshare was right
                    predicted_right++;
                } else if(gshare_right == 0 && bimodal_right == 1){
                    //gshare was wrong
                    //change state to weakly gshare
                    selector_table[selector_and_bimodal_index] = 1;
                } else if (gshare_right == 0 && bimodal_right == 0){
                    //both are wrong
                    //nothing happens
                }

                break;
            case 1:
                //Weakly Gshare
                gshare_helper(gshare_table, token, gshare_index, &history, &gshare_right);
                two_bit_helper(bimodal_table, token, &bimodal_right, selector_and_bimodal_index);
                if(gshare_right == 1 && bimodal_right == 1){
                    //both predictors are correct
                    predicted_right++;
                } else if(gshare_right == 1 && bimodal_right == 0){
                    //only gshare was right
                    predicted_right++;
                    //move to strongly shared
                    selector_table[selector_and_bimodal_index] = 0;
                } else if(gshare_right == 0 && bimodal_right == 1){
                    //gshare was wrong
                    //change state to weakly bimodal
                    selector_table[selector_and_bimodal_index] = 2;
                } else if (gshare_right == 0 && bimodal_right == 0){
                    //both are wrong
                    //nothing happens
                }
                break;
            case 2:
                //Weakly Bimodal
                gshare_helper(gshare_table, token, gshare_index, &history, &gshare_right);
                two_bit_helper(bimodal_table, token, &bimodal_right, selector_and_bimodal_index);
                if(gshare_right == 1 && bimodal_right == 1){
                    //both predictors are correct
                    predicted_right++;
                } else if(gshare_right == 1 && bimodal_right == 0){
                    //only gshare was right
                    //move to weakly gshared
                    selector_table[selector_and_bimodal_index] = 1;
                } else if(gshare_right == 0 && bimodal_right == 1){
                    //bimodal was right
                    predicted_right++;
                    //change state to strongly bimodal
                    selector_table[selector_and_bimodal_index] = 3;
                } else if (gshare_right == 0 && bimodal_right == 0){
                    //both are wrong
                    //nothing happens
                }
                break;
            case 3:
                //Strongly Bimodal
                gshare_helper(gshare_table, token, gshare_index, &history, &gshare_right);
                two_bit_helper(bimodal_table, token, &bimodal_right, selector_and_bimodal_index);

                if(gshare_right == 1 && bimodal_right == 1){
                    //both predictors are correct
                    predicted_right++;
                } else if(gshare_right == 1 && bimodal_right == 0){
                    //only gshare was right
                    //move to weakly bimodal
                    selector_table[selector_and_bimodal_index] = 2;
                } else if(gshare_right == 0 && bimodal_right == 1){
                    //bimodal was right
                    predicted_right++;
                    //change state to strongly bimodal
                } else if (gshare_right == 0 && bimodal_right == 0){
                    //both are wrong
                    //nothing happens
                }
                break;
            default:
                fprintf(stderr, "Unknown error in Tournament Predictor.\n");
                exit(1);
            }

        //reset correct counters for both predictors
        gshare_right = 0;
        bimodal_right = 0;

        total_counter++;
    }
    *out << predicted_right << "," << total_counter << ";";
    //printf("%d,%d;\n", predicted_right, total_counter);

}

void two_bit_helper(int table[], char* branch, int* predicted_right, int index){
    int state = table[index];

    if(strcmp(branch, "NT") == 0){
        if(state == 0){
            //Strongly NonTaken
            (*predicted_right)++;
        } else if(state == 1){
            //Weakly NonTaken
            (*predicted_right)++;
            table[index] = 0;
        } else if(state == 2){
            //Weakly Taken
            //didn't predict right
            table[index] = 1;
        } else if (state == 3){
            //Strongly Taken
            //didn't predict right
            table[index] = 2;
        } else {
            fprintf(stderr, "Something went wrong with the Two Bit Predictor.\n");
            exit(1);
        }
    } else if(strcmp(branch, "T") == 0){
        if(state == 0){
            table[index] = 1;
        } else if (state == 1){
            table[index] = 2;
        } else if(state == 2){
            (*predicted_right)++;
            table[index] = 3;
        } else if (state == 3){
            (*predicted_right)++;
        } else {
            fprintf(stderr, "Something went wrong with the Two Bit Predictor.\n");
            exit(1);
        }
    }
}

unsigned int get_mask(unsigned int size){
    //create a mask based off of size of the history size
    unsigned int retVal = 0;
    switch(size)
        {
        case 3:
            retVal = 0b0000000000000111;
            break;
        case 4:
            retVal = 0b0000000000001111;
            break;
        case 5:
            retVal = 0b0000000000011111;
            break;
        case 6:
            retVal = 0b0000000000111111;
            break;
        case 7:
            retVal = 0b0000000001111111;
            break;
        case 8:
            retVal = 0b0000000011111111;
            break;
        case 9:
            retVal = 0b0000000111111111;
            break;
        case 10:
            retVal = 0b0000001111111111;
            break;
        case 11:
            retVal = 0b0000011111111111;
            break;
        default:
            fprintf(stderr, "Unknown error in gshare predictor.\n");
            exit(1);
        }
    return retVal;
}

void gshare_helper(int table[], char* branch, int index, unsigned int* history, int* predicted_right){

    int state = table[index];

    if(strcmp(branch, "NT") == 0){
        if(state == 0){
            //Strongly NonTaken
            (*predicted_right)++;

            //pad with 0
            *history <<= 1;
        } else if(state == 1){
            //Weakly NonTaken
            (*predicted_right)++;
            table[index] = 0;
            //pad with 0
            *history <<= 1;
        } else if(state == 2){
            //Weakly Taken
            //didn't predict right

            //pad with 0
            *history <<= 1;

            //go to Weak NT
            table[index] = 1;
        } else if (state == 3){
            //Strongly Taken
            //didn't predict right

            //pad with 0
            *history <<= 1;

            //go to Weak T
            table[index] = 2;
        } else {
            fprintf(stderr, "Something went wrong with the Gshare Predictor.\n");
            exit(1);
        }

    } else if(strcmp(branch, "T") == 0){
        if(state == 0){
            //Strongly NonTaken

            //add one at the end of the history
            *history <<= 1;
            *history |= 1;

            //go to Weakly NonTaken
            table[index] = 1;
        } else if (state == 1){
            //Weakly NonTaken

            //add one at the end of the history
            *history <<= 1;
            *history |= 1;

            //go to Weakly Taken State
            table[index] = 2;
        } else if(state == 2){
            // Weakly Taken
            (*predicted_right)++;

            //add one to history
            *history <<= 1;
            *history |= 1;

            //go to Strongly Taken
            table[index] = 3;
        } else if (state == 3){
            //Strongly Taken
            (*predicted_right)++;

            //add one to history
            *history <<= 1;
            *history |=1;
        } else {
            fprintf(stderr, "Something went wrong with the Gshare Predictor.\n");
            exit(1);
        }
    }
}

void gshare(int history_size, ifstream* in, ofstream* out){
    string line;
    int table[2048];
    const char delim[2] = " ";
    char* token;
    int predicted_right = 0;
    int counter = 0;
    unsigned int history = 0;
    unsigned int mask = 0;

    //create a mask based off of size of the history size
    mask = get_mask(history_size);

    //initialize table to TT
    for(int i = 0; i < 2048; i++){
        table[i] = 3;
    }

    while(getline(*in, line)){
        char* new_line = (char*)line.c_str();
        token = strtok(new_line, delim);
        unsigned int address = hexadecimalToDecimal(token);
        unsigned int new_history = history & mask;
        int index = (address % 2048) ^ new_history;

        token = strtok(NULL, delim);

        gshare_helper(table, token, index, &history, &predicted_right);

        counter++;
    }

    *out << predicted_right << "," << counter << "; ";
    //printf("%d,%d; ", predicted_right, counter);

}

void two_bit(int size, std::ifstream* in, ofstream* out){
    std::string line;
    int table[size];
    const char delim[2] = " ";
    char* token;
    int predicted_right = 0;
    int counter = 0;

    //initialize table to Taken (1).
    for (int i = 0; i < size; i++){
        table[i] = 3;
    }

    while(getline(*in, line)){
        char* new_line = (char*)line.c_str();
        token = strtok(new_line, delim);
        unsigned int address = hexadecimalToDecimal(token);
        int index = address % size;

        token = strtok(NULL, delim);

        two_bit_helper(table, token, &predicted_right, index);

        counter++;
    }

    *out << predicted_right << "," << counter << "; ";
    //printf("%d,%d; ", predicted_right, counter);
}

void one_bit(int size, std::ifstream* in, ofstream* out){
    std::string line;
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
        unsigned int address = hexadecimalToDecimal(token);
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

    *out << predicted_right << "," << counter << "; ";
    //printf("%d,%d; ", predicted_right, counter);
}

void always(std::string n_t, std::ifstream* in, ofstream* out){
    std::string line;
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
    //TODO: make it out put to file DONE
    *out << predicted_right << "," << counter << ";" << endl;
    //printf("%d,%d;\n", predicted_right, counter);
}

//retrieved function from:
// https://www.geeksforgeeks.org/program-hexadecimal-decimal/
unsigned int hexadecimalToDecimal(char hexVal[]) {
    for(unsigned int i = 2; i < strlen(hexVal); i++){
        hexVal[i-2] = hexVal[i];
    }
    hexVal[strlen(hexVal) - 2] = '\0';
    int len = strlen(hexVal);
    // Initializing base value to 1, i.e 16^0
    int base = 1;
    unsigned int dec_val = 0;

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


void print_binary(unsigned short n) {
    unsigned i;
    for (i = 1 << 31; i > 0; i = i / 2)
        (n & i)? printf("1"): printf("0");
}
