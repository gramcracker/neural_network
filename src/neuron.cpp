#include <iostream>
#include "neuron.h"
#include <vector>
#include <cmath>
#include <thread>
#include <set>

using namespace std;


neuron::~neuron()
{
    cout<<endl<<"neuron destructor called";
	int j=0;
		for (  vector <link*>::iterator i=inweights.begin (); i !=inweights.end(); i++)
		{
			delete (*i);
			cout<<endl <<this<<"'s link number "<<j <<" deleted";
		}
		inweights.clear ();
	cout << endl <<"The size of the vector is " << inweights.size();
}



void neuron::setin(double input)
{
    _input=input;   // function for setting input
}


void neuron::activate() //the sigmoid activation function
{
    switch(_buffer)
    {
    case 0.0:
        _buffer=.5;
        break;
    case 1.0:
        _buffer=.73105857;
        break;
    case 0.5:
        _buffer=.62245933;
        break;
    default:
        _buffer=(1/(1+(exp(-(_buffer)))));
        information.insert(_buffer);
    }

}


double neuron::getval()
{
    return(_value);
}

void neuron:: set_bias(double bias)
  {
	  _bias=bias;
  }


void neuron::feed()// feed forward a single neuron
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

void neuron::reconstruct()// feed forward a single neuron
{

    for(unsigned int i=0; i<inweights.size(); i++)
    {
        inweights[i]->_origin._buffer+=_value*inweights[i]->_weight;

    }


}



void neuron::throwup()
{
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































