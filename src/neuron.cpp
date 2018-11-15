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
        for (  vector <link*>::iterator i=inweights.begin (); i !=inweights.end(); i++)
        {
            delete (*i);
        }
        inweights.clear ();

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
    for(int i=0; i<inweights.size();i++)
    {
        inweights[i]->_weight=val;
    }
}


/**
 * @brief      retrieves and adds the values from preceding linked neurons then activates this neuron.
 *
 * @param[in]  activationtype  The activationtype
 */
// void neuron::feed(ActivationType activationtype)// feed forward a single neuron
// {
//     if(inweights.size()==0)
//     {
//         _buffer=_input;
//     }
//     else
//     {
//         for(unsigned int i=0; i<inweights.size(); i++)
//         {
//             _buffer+=((inweights[i]->_weight)*(inweights[i]->_origin._value))+_bias;
//         }


//         _buffer+=_input;
//         activate(activationtype);

//     }
// }

// void neuron::reconstruct(ActivationType activationtype)
// {
//     if(inweights.size()==0)
//     {
//         _buffer=_input;
//     }
//     else
//     {
//         for(unsigned int i=0; i<inweights.size(); i++)
//         {
//             _buffer+=((inweights[i]->_weight)*(inweights[i]->_origin._value))+_bias;
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
    if(inweights.size()==0)
    {
        _buffer=_input;
    }
    else
    {
        for(unsigned int i=0; i<inweights.size(); i++)
        {
            _buffer+=((inweights[i]->_weight)*(inweights[i]->_origin._value))+_bias;
        }


        _buffer+=_input;
        activate();

    }
}

void neuron::feed(ActivationType activationtype, *unordered_set<*neuron> activeSubset){
    activeSubset->insert(this);
    feed(activationtype)
}


/**
 * @brief      sets the value of the neuron to zero
 */
void neuron::clear()
{

    _buffer=0;
}

//Todo: switch origin name to destination
void neuron::feed(ActivationType activationtype){

    activate(activationtype); 
    //only multiply and feed value foreward if !=0
    if(inweights.size()>0 && _value != 0){
        for(unsigned int i=0; i<inweights.size(); i++)
        {
            inweights[i]->_origin._buffer+=_value*inweights[i]->_weight;
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
    if(inweights.size()>0)
    {
        for(unsigned int i=0; i<inweights.size(); i++)
        {
            inweights[i]->_origin._ebuffer+=(inweights[i]->_weight)*(_gradient);//pass the error before weight change
            inweights[i]->_old_weight=inweights[i]->_weight;
            inweights[i]->_weight-=_gradient*(inweights[i]->_origin._value);
            inweights[i]->_origin._desired=(inweights[i]->_origin._value);


        }
    }
}




























