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

    always(T, &input, &output);

    input.clear();
    input.seekg(0);

    cout << "Finished Always Taken...\n";

    always(NT, &input, &output);

    input.clear();
    input.seekg(0);

    cout << "Finished Always Non-Taken...\n";

    //==========================================================================
    //Bimodal Predictor wiht a single bit of history
    //myFile.seekp(-1, std::ios_base::cur);
    one_bit(16, &input, &output);

    input.clear();
    input.seekg(0);

    one_bit(32, &input, &output);

    input.clear();
    input.seekg(0);

    one_bit(128, &input, &output);

    input.clear();
    input.seekg(0);

    one_bit(256, &input, &output);

    input.clear();
    input.seekg(0);

    one_bit(512, &input, &output);

    input.clear();
    input.seekg(0);

    one_bit(1024, &input, &output);

    input.clear();
    input.seekg(0);


    one_bit(2048, &input, &output);

    input.clear();
    input.seekg(0);

    output.seekp(-1, std::ios_base::cur);
    output<<endl;

    cout << "Finished One Bit Bimodal ...\n";


    //==========================================================================
    //Bimodal Predictor with 2-bit saturating counters

    two_bit(16, &input, &output);

    input.clear();
    input.seekg(0);

    two_bit(32, &input, &output);

    input.clear();
    input.seekg(0);

    two_bit(128, &input, &output);

    input.clear();
    input.seekg(0);

    two_bit(256, &input, &output);

    input.clear();
    input.seekg(0);

    two_bit(512, &input, &output);

    input.clear();
    input.seekg(0);


    two_bit(1024, &input, &output);

    input.clear();
    input.seekg(0);

    two_bit(2048, &input, &output);

    input.clear();
    input.seekg(0);

    output.seekp(-1, std::ios_base::cur);
    output << endl;

    cout << "Finished Two Bit Bimodal...\n";

    //==========================================================================
    // Gshare Predictor

    gshare(3, &input, &output);

    input.clear();
    input.seekg(0);

    gshare(4, &input, &output);

    input.clear();
    input.seekg(0);

    gshare(5, &input, &output);

    input.clear();
    input.seekg(0);

    gshare(6, &input, &output);

    input.clear();
    input.seekg(0);

    gshare(7, &input, &output);

    input.clear();
    input.seekg(0);

    gshare(8, &input, &output);

    input.clear();
    input.seekg(0);

    gshare(9, &input, &output);

    input.clear();
    input.seekg(0);

    gshare(10, &input, &output);

    input.clear();
    input.seekg(0);

    gshare(11, &input, &output);

    input.clear();
    input.seekg(0);

    output.seekp(-1, std::ios_base::cur);
    output << endl;
    cout << "Finished Gshare...\n";

    //==========================================================================
    //Tournament Predictor

    tournament(&input, &output);
    output << endl;
    cout << "Finished Tournament!" << endl;
    return 0;
}

