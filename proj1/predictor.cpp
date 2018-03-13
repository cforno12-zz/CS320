#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include "branches.h"

using namespace std;

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

    // always(T, &input);

    // input.clear();
    // input.seekg(0);

    // always(NT, &input);

    // input.clear();
    // input.seekg(0);

    //==========================================================================
    //Bimodal Predictor wiht a single bit of history

    // one_bit(16, &input);

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

    // cout << endl;

    //==========================================================================
    //Bimodal Predictor with 2-bit saturating counters

    // two_bit(16, &input);

    // input.clear();
    // input.seekg(0);

    // two_bit(32, &input);

    // input.clear();
    // input.seekg(0);

    // two_bit(128, &input);

    // input.clear();
    // input.seekg(0);

    // two_bit(256, &input);

    // input.clear();
    // input.seekg(0);

    // two_bit(512, &input);

    // input.clear();
    // input.seekg(0);


    // two_bit(1024, &input);

    // input.clear();
    // input.seekg(0);

    // two_bit(2048, &input);

    // input.clear();
    // input.seekg(0);

    // cout << endl;

    //==========================================================================
    // Gshare Predictor

    gshare(3, &input);



    return 0;
}

