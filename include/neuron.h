#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include <set>
#include "link.h"
using namespace std;

class link;

class neuron
{

public:
    //this class is only to be constructed indirectly using automatically constructed nets
    //or functions that create a new instance of this class
    //any other implementation will cause the instance to be deleted twice
	~neuron();
    vector <link*> inweights; //vector of links coming in. you must push back the links to wire the net
    double _desired=0;// desired output
    double learning_rate=7.5; //learning rate

    virtual void activate();
    void feed(); // feed forward for this neuron
    void get_entropy();//calculates the amount of information received by the neuron
    double getval();//returns value of the neuron
    void reconstruct();//same as feed foreward but in the opposite direction
    void set_bias(double bias);
    void setin(double input);// function for setting input
    virtual void throwup();//back propagates for this neuron



    friend class net;
    friend class lstm_block;// a father must have a close relationship with his offspring
    double _buffer=0;// buffer for value to ensure correct timing of activation
    double _ebuffer=0;// buffer for error propagation
protected:
    multiset<float> information;//multi-sets can give the number of individual values

    double _gradient=0;
    double _value=0; //the input from array of links or weights
    double _input=0;//input into the network
    double _error=0;//the difference between desired output and the real output
    int entropy=0;// used to give the number of neurons added above this neuron
    double _bias=0;//self explanatory
    double _old_value=0;//previous value of neuron

};




#endif
