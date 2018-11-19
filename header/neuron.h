#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include <set>
#include "link.h"
#include <iostream>
#include <string>
#include <unordered_set>


using namespace std;

class link;



class neuron
{



public:

     //this class is only to be constructed indirectly using automatically constructed nets
    //or functions that create a new instance of this class
    //any other implementation will cause the instance to be deleted twice

    ~neuron();
    vector <link*> outweights; //vector of links coming in. you must push back the links to wire the net
    double _desired=0;// desired output
    double learning_rate=7.5; //learning rate
    enum ActivationType{ none=0, sigmoid, step, tanH, fast_sigmoid };
    enum LearningType{ backpropagation=0, contrastive_divergence, hebbian, convolutional, copy_values, singly_recurrent, winner_take_all};
    ActivationType activation_type=fast_sigmoid;
    double   activation_limit=4.9;
    virtual void activate(ActivationType activationtype=fast_sigmoid);
    void feed(ActivationType activationtype=fast_sigmoid);
    void ffeed( unordered_set <neuron*>& activeSubset);
    void feed(ActivationType activationtype, unordered_set <neuron*>& activeSubset, double threshold);
    void clear();
    void activate_hormonal();
    void get_entropy();//calculates the amount of information received by the neuron
    double getval();//returns value of the neuron
    void reconstruct();//same as feed foreward but in the opposite direction
    void set_bias(double bias);
    void setin(double input);// function for setting input
    void set_weights(double val);
    void set_rand_weights();
    virtual void throwup();//back propagates for this neuron
    //virtual void throwup(LearningType leaning_type);

    //feed alternatives
    void repeat();//repeats the value



    friend class net;
    friend class lstm_block;// a father must have a close relationship with his offspring
    double _buffer=0;// buffer for value to ensure correct timing of activation
    double _ebuffer=0;// buffer for error propagation

    //used for saving position of neurons
    int row;
    int col;

protected:
    multiset<float> information;//multi-sets can give the number of individual values

    double _gradient=0;
    double _value=0; //the input from array of links or weights
    double _input=0;//input into the network
    double _error=0;//the difference between desired output and the real output
    int entropy=0;// used to give the number of neurons added above this neuron
    double _bias=0;//self explanatory
    double _old_value=0;//previous value of neuron
    double logistic[101]={
  0.0066929, 0.0073915, 0.0081626, 0.0090133, 0.0099518,
  0.010987, 0.012128, 0.013387, 0.014774, 0.016302,
  0.017986, 0.01984, 0.021881, 0.024127, 0.026597,
  0.029312, 0.032295, 0.035571, 0.039166, 0.043107,
  0.047426, 0.052154, 0.057324, 0.062973, 0.069138,
  0.075858, 0.083173, 0.091123, 0.09975, 0.1091,
  0.1192, 0.13011, 0.14185, 0.15447, 0.16798,
  0.18243, 0.19782, 0.21417, 0.23148, 0.24974,
  0.26894, 0.28905, 0.31003, 0.33181, 0.35434,
  0.37754, 0.40131, 0.42556, 0.45017, 0.47502,
  0.5, 0.52498, 0.54983, 0.57444, 0.59869,
  0.62246, 0.64566, 0.66819, 0.68997, 0.71095,
  0.73106, 0.75026, 0.76852, 0.78583, 0.80218,
  0.81757, 0.83202, 0.84553, 0.85815, 0.86989,
  0.8808, 0.8909, 0.90025, 0.90888, 0.91683,
  0.92414, 0.93086, 0.93703, 0.94268, 0.94785,
  0.95257, 0.95689, 0.96083, 0.96443, 0.9677,
  0.97069, 0.9734, 0.97587, 0.97812, 0.98016,
  0.98201, 0.9837, 0.98523, 0.98661, 0.98787,
  0.98901, 0.99005, 0.99099, 0.99184, 0.99261,
  0.99331};

};





#endif
