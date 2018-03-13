#include <string.h>
#include <fstream>

#ifndef BRAN_H
#define BRAN_H

unsigned int hexadecimalToDecimal(char hexVal[]);
void print_binary(unsigned n);
void always(std::string n_t, std::ifstream* in);
void one_bit(int size, std::ifstream* in);
void two_bit(int size, std::ifstream* in);
void gshare(int history, std::ifstream* in);

#endif
