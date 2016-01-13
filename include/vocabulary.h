#ifndef VOCABULARY_H
#define VOCABULARY_H
#include <vector>
#include <iostream>
#include <string>
#include "net.h"
using namespace std;

class vocabulary
{
    public:
        vocabulary(net& Net):_Net(Net){};// adds the vocabulary object to a network
        void parse();//used to pass text to the read function using a terminal
        void read(string word); // passes words to a "window" like in a convoluted net. ( eventually will also set value of corresponding neurons to 1.0)
        void insert(string word);// maps words to neurons. every new word gets a new neuron.
        void display();

        vector<string> word_vec;
        vector<string> window;
        string mystring, temp;
        int window_size=5;

    private:
        net& _Net;

};

#endif // VOCABULARY_H
