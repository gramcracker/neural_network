#include <iostream>
#include "neuron.h"
#include <vector>
#include <cmath>
#include <thread>
#include <set>

using namespace std;


/**
 * @brief      Destroys the object.
 */
neuron::~neuron()
{
    //cout<<"deleting: "<<this<<endl;
    int j=0;
        for (  vector <link*>::iterator i=outweights.begin (); i !=outweights.end(); i++)
        {
            delete (*i);
        }
        outweights.clear ();

}


/**
 * @brief      sets the input
 *
 * @param[in]  input  The input
 */
void neuron::setin(double input)
{
    _buffer=input;   // function for setting input
}



/**
 * @brief      activates the neuron
 *
 * @param[in]  activationtype  The activationtype
 */
void neuron::activate(ActivationType activationtype)// activation function using a specific Activation type
{
    _buffer+=_bias;
    switch(activationtype){
    case none:
        break;
    case sigmoid:
        //_buffer stores the value until whole layer is finished processing,
        if(_buffer==0.0){_buffer=.5;}
        else if(_buffer==1.0){_buffer=.73105857;}
        else if(_buffer==0.5){_buffer=.62245933;}
        else{
        _buffer=(1/(1+(exp(-(_buffer)))));
        }
        break;
    case step:
        _buffer>0?_buffer=1:_buffer=0;
        break;
    case tanH:
        _buffer= tanh(_buffer);//precision is lost here
        break;
    case fast_sigmoid:
        if (_buffer<=-activation_limit) _buffer=0;
    else if (_buffer>=activation_limit) _buffer=1;
    else _buffer=logistic[(int)(_buffer*10+50)];
        break;   
    }
    _value = _buffer;
    //information.insert(_buffer);

}

/**
 * @brief      activates the neuron based on hormones
 */
void neuron::activate_hormonal()
{

}


/**
 * @brief      returns the current value of the neuron
 *
 * @return     the neurons value
 */
double neuron::getval()
{
    return(_value);
}


/**
 * @brief      Sets the bias.
 *
 * @param[in]  bias  The bias
 */
void neuron::set_bias(double bias)
  {
      _bias=bias;
  }



/**
 * @brief      Sets the weights.
 *
 * @param[in]  val   The value
 */
void neuron::set_weights(double val)
{
    for(int i=0; i<outweights.size();i++)
    {
        outweights[i]->_weight=val;
    }
}


/**
 * @brief      retrieves and adds the values from preceding linked neurons then activates this neuron.
 *
 * @param[in]  activationtype  The activationtype
 */
// void neuron::feed(ActivationType activationtype)// feed forward a single neuron
// {
//     if(outweights.size()==0)
//     {
//         _buffer=_input;
//     }
//     else
//     {
//         for(unsigned int i=0; i<outweights.size(); i++)
//         {
//             _buffer+=((outweights[i]->_weight)*(outweights[i]->postsynaptic._value))+_bias;
//         }


//         _buffer+=_input;
//         activate(activationtype);

//     }
// }

// void neuron::reconstruct(ActivationType activationtype)
// {
//     if(outweights.size()==0)
//     {
//         _buffer=_input;
//     }
//     else
//     {
//         for(unsigned int i=0; i<outweights.size(); i++)
//         {
//             _buffer+=((outweights[i]->_weight)*(outweights[i]->postsynaptic._value))+_bias;
//         }
//         _buffer+=_input;
//         activate(activationtype);
//     }
// }

/**
 * @brief      retrieves and adds the values from preceding linked neurons then activates this neuron.
 */
void neuron::reconstruct()
{
    if(outweights.size()==0)
    {
        _buffer=_input;
    }
    else
    {
        for(unsigned int i=0; i<outweights.size(); i++)
        {
            _buffer+=((outweights[i]->_weight)*(outweights[i]->postsynaptic._value))+_bias;
        }


        _buffer+=_input;
        activate();

    }
}

void neuron::ffeed(unordered_set<neuron*> &activeSubset){
    activate(neuron::fast_sigmoid); 
    //only multiply and feed value foreward if !=0
    if(outweights.size()>0 && _value != 0){
        for(unsigned int i=0; i<outweights.size(); i++)
        {
            outweights[i]->postsynaptic._buffer+=_value*outweights[i]->_weight;
            activeSubset.insert(&(outweights[i]->postsynaptic));
        }
    }
}

void neuron::feed(ActivationType activationtype, unordered_set<neuron*> &activeSubset, double threshold){
    activate(activationtype); 
    //only multiply and feed value foreward if !=0
    if(outweights.size()>0 && _value != 0){
        for(unsigned int i=0; i<outweights.size(); i++)
        {
            outweights[i]->postsynaptic._buffer+=_value*outweights[i]->_weight;
            if(outweights[i]->postsynaptic._buffer> threshold){
                activeSubset.insert(&(outweights[i]->postsynaptic));
            }
        }
    }
}


/**
 * @brief      sets the value of the neuron to zero
 */
void neuron::clear()
{

    _buffer=0;
}


void neuron::feed(ActivationType activationtype){

    activate(activationtype); 
    //only multiply and feed value foreward if !=0
    if(outweights.size()>0 && _value != 0){
        for(unsigned int i=0; i<outweights.size(); i++)
        {
            outweights[i]->postsynaptic._buffer+=_value*outweights[i]->_weight;
        }
    }

}


//TODO rename this
/**
 * @brief      to get the error and pass it back
 */
void neuron::throwup()
{
    //todo: ,ake sure this works
    _error=(_value-_desired)+_error;
    _gradient=((learning_rate)*(_error)*((_value)*(1-(_value))));//calculate error for neuron
    if(outweights.size()>0)
    {
        for(unsigned int i=0; i<outweights.size(); i++)
        {
            outweights[i]->postsynaptic._ebuffer+=(outweights[i]->_weight)*(_gradient);//pass the error before weight change
            outweights[i]->_old_weight=outweights[i]->_weight;
            outweights[i]->_weight-=_gradient*(outweights[i]->postsynaptic._value);
            outweights[i]->postsynaptic._desired=(outweights[i]->postsynaptic._value);


        }
    }
}




























