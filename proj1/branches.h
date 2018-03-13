#include <string.h>
#include <fstream>

#ifndef BRAN_H
#define BRAN_H

void gshare_helper(int table[], char* branch, int index, unsigned int* history, int* predicted_right);
unsigned int get_mask(unsigned int size);
void two_bit_helper(int table[], char* branch, int* predicted_right, int index);
unsigned int hexadecimalToDecimal(char hexVal[]);
void print_binary(unsigned n);
void always(std::string n_t, std::ifstream* in);
void one_bit(int size, std::ifstream* in);
void two_bit(int size, std::ifstream* in);
void gshare(int history, std::ifstream* in);

#endif
