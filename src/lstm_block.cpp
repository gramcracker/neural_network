#include "lstm_block.h"
#include <cmath>

	void lstm_block::feed ()
	{


		for (int i=0; i <4; i++)
		{
			layer1 [i]*=((inweights[i]->_weight)*(inweights[i]->_origin._value))+_bias;

	layer1[i]=(1/(1+(exp(-(layer1 [i])))));

		}



	}

void lstm_block::activate()
	{
		n2=n0+n1;
		n1*=n2;
		n3*=n2;
		_value=n3;
	};
