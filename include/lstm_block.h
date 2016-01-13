#ifndef LSTM_BLOCK_H
#define LSTM_BLOCK_H
#include "neuron.h"

class lstm_block: public neuron
{
    public:
        void feed();
        void activate();

    private:
        double n0=1,n1=1,n2=0,n3=1;
        double layer1[4]={n0,n0,n1,n3};
};

#endif // LSTM_BLOCK_H
