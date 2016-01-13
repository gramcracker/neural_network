#include <iostream>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <thread>
#include "net.h"

using namespace std;

//
net::net(){};

//constructor
net::net(vector <int> &dimensions)
{
    layers.resize(dimensions.size());
		cout<<endl<<"size = "<<dimensions.size();
		for (int i=0; i <dimensions.size(); i++)
		{
			for (int j=0; j <dimensions[i]; j++)
			{
				layers.at(i).push_back(new neuron);

				cout<<endl<<"new neuron at "<<i<<","<<j;

			}
		}
};
//constructor for fully connected
net::net(vector <int> &dimensions, bool fullyconnected)
{
    layers.resize(dimensions.size());
		cout<<endl<<"size = "<<dimensions.size();
		for (int i=0; i <dimensions.size(); i++)
		{
			for (int j=0; j <dimensions[i]; j++)
			{
				layers.at(i).push_back(new neuron);

				cout<<endl<<"new neuron at "<<i<<","<<j;
				if (i>=1&&(fullyconnected==true))
					{
						for (int k=0; k <layers [i-1].size();k++)
						{

							layers[i][j]->inweights.push_back (new link(*(layers [i-1][k])));
							cout<<endl<<"new link from "<<(i-1)<<","<<k<<" to "<<i<<","<<j;

						}
					}
			}
		}
};

//destructor
net::~net()
	{

		for(row=layers.begin(); row!=layers.end(); row++)
		{

			for(col=row->begin(); col !=row->end(); col++)
			{


				delete(*col); //deletes the node
				cout <<endl<<"deleted neuron";
			}
			row->clear();//clear the row it points too
		}
	layers.clear();

	}

//copy constructor
net::net(const net& tobecloned)
{
    int y;
    int x;

    for(row=layers.begin(); row!=layers.end(); row++)
    {

        for(col=row->begin(); col !=row->end(); col++)
        {

            delete(*col); //deletes the node
            cout <<endl<<"deleted neuron";
        }
        row->clear();//clear the row it points too
    }
    layers.clear();


    layers.resize(tobecloned.layers.size());
    for (int i=0; i<tobecloned.layers.size()-1; i++)
    {
        for(int j=0; j<tobecloned.layers[i].size(); j++)
        {

            tobecloned.layers[i][j]->_buffer=i;
            tobecloned.layers[i][j]->_ebuffer=j;
            layers[i].push_back(new neuron);

            for(int k=0; k<tobecloned.layers[i][j]->inweights.size(); k++)
            {
                y=tobecloned.layers[i][j]->inweights[k]->_origin._buffer;
                x=tobecloned.layers[i][j]->inweights[k]->_origin._ebuffer;
                layers[i][j]->inweights.push_back(new link(*(layers[y][x])));
            }

        }

    }
}


// overloaded assignment operator
net& net::operator=(const net& tobecloned)
	{
        int y;
        int x;


        for(row=layers.begin(); row!=layers.end(); row++)
		{

			for(col=row->begin(); col !=row->end(); col++)
			{


				delete(*col); //deletes the node
				cout <<endl<<"deleted neuron";
			}
			row->clear();//clear the row it points too
		}
	layers.clear();


        layers.resize(tobecloned.layers.size());
        for (int i=0; i<tobecloned.layers.size()-1; i++)
        {
            for(int j=0; j<tobecloned.layers[i].size(); j++)
            {

                tobecloned.layers[i][j]->_buffer=i;
                tobecloned.layers[i][j]->_ebuffer=j;
                layers[i].push_back(new neuron);

                for(int k=0; k<tobecloned.layers[i][j]->inweights.size(); k++)
                {
                    y=tobecloned.layers[i][j]->inweights[k]->_origin._buffer;
                    x=tobecloned.layers[i][j]->inweights[k]->_origin._ebuffer;
                    layers[i][j]->inweights.push_back(new link(*(layers[y][x])));
                }

            }

    }


	    return (*this);



	}



int net:: get_size()
{
    return layers.size();
}

int net::get_size(int i)
{

    return layers[i].size();
}

void net::SetInput(vector<double>& input)
{
	for( int i=0; i<layers.at(0).size(); i++)
	{
        layers.at(0).at(i)->_input=(input[i]);
	};
}




void net::SetDesired(vector<double>& desired)
{
	for( int x=0; x<layers.at(layers.size()-1).size(); x++)
	{
        layers.at(layers.size ()-1).at(x)->_desired=(desired[x]);
	};
}



void net::FeedForeward()
{

	for(row=layers.begin(); row!=layers.end(); row++)
	{

		for(col=row->begin(); col !=row->end(); col++)
		{
			threads.push_back(thread(&neuron::feed,(*col)));
		}
		for(int i=0; i<threads.size(); i++)
		{
			threads[i].join();
		}

		threads.erase(threads.begin(),threads.end());

		if (threads.size()>0){cout<<endl<<threads.size()<<"threads remain";}

		for(col=row->begin(); col !=row->end(); col++)
		{
			((*col)->_value)=((*col)->_buffer);//to avoid accessing and activating a neuron at the same time ( like if a neuron is connected to one in the same layer)
		}

	}

}




void net::FeedForeward( int starting_layer, int ending_layer)
{

	for(row=(layers.begin()+starting_layer); row!=(layers.begin()+ending_layer+1); row++)
	{
		for(col=row->begin(); col !=row->end(); col++)
		{
			threads.push_back(thread(&neuron::feed,(*col)));
		}
		for(int i=0; i<threads.size(); i++)
		{
			threads[i].join();
		}

		threads.erase(threads.begin(),threads.end());

		if (threads.size()>0){cout<<endl<<threads.size()<<"threads remain";}

		for(col=row->begin(); col !=row->end(); col++)
		{
			((*col)->_value)=((*col)->_buffer);//to avoid accessing and activating a neuron at the same time ( like if a neuron is connected to one in the same layer)
		}

	}

}



void net::Backpropagate()
{


	for(rrow=layers.rbegin(); rrow!=layers.rend(); rrow++)
	{
		for(rcol=rrow->rbegin(); rcol !=rrow->rend(); rcol++)
		{
			(*rcol)->_error=(*rcol)->_ebuffer;//to avoid accessing and propagating to a neuron at the same time ( like if a neuron is connected to one in the same layer)
		}

		for(rcol=rrow->rbegin(); rcol !=rrow->rend(); rcol++)
		{
			threads.push_back(thread(&neuron::throwup,(*rcol)));
		}

		for(int i=0; i<threads.size(); i++)
		{
			threads[i].join();

		}


		threads.erase(threads.begin(),threads.end());//destroys threads in array to leave it empty for next layer


		if (threads.size()>0){cout<<endl<<threads.size()<<"threads remain";}
	}
}




void net::Train(vector <double>& input, vector <double>& desired)
{
	SetInput(input);

	FeedForeward();

	SetDesired(desired);

	Backpropagate();

}

void net::ContrastDiverge( int starting_layer)
{


	FeedForeward(starting_layer, starting_layer+1);

	for(row=(layers.begin()+starting_layer); row!=(layers.begin()+(starting_layer+2)); row++)
	{
		for(col=row->begin(); col !=row->end(); col++)
		{
			(*col)->_old_value=(*col)->_value;

			(*col)->_value>=.5?(*col)->_value=1:(*col)->_value=0;


			for(unsigned int i=0;i<((*col)->inweights.size());i++)
			{
				(*col)->inweights[i]->_positive_phase=((*col)->_value)*((*col)->inweights[i]->_origin._value);
				cout<<endl<<"positive phase ="<<(*col)->inweights[i]->_positive_phase;
			}
		}

	}

	for(col=(layers.at(starting_layer+1)).begin(); col !=((layers.at(starting_layer+1)).end()); col++)
	{


	(*col)->_value=(*col)->_buffer;

		threads.push_back(thread(&neuron::reconstruct,(*col)));

	}

	for(int i=0; i<threads.size(); i++)
	{

		threads[i].join();

	}

	threads.erase(threads.begin(),threads.end());//destroys threads in array to leave it empty for next layer

	for(col=(layers.at(starting_layer)).begin(); col !=(layers.at(starting_layer)).end(); col++)
	{
		(*col)->activate();
		(*col)->_value=(*col)->_buffer;
		if(starting_layer==0)
		{
				(*col)->_input=(*col)->_value;
		}
	}

	FeedForeward(starting_layer, starting_layer+1);

	for(row=(layers.begin()+starting_layer); row!=(layers.begin()+(starting_layer+2)); row++)
	{
		for(col=row->begin(); col !=row->end(); col++)
		{

			(*col)->_bias+=((*col)->_old_value)-((*col)->_value);
		// get bias before value is changed
			(*col)->_value>=.5?(*col)->_value=1:(*col)->_value=0;


			for(unsigned int i=0;i<((*col)->inweights.size());i++)
			{

				(*col)->inweights[i]->_negative_phase=((*col)->_value)*((*col)->inweights[i]->_origin._input);
				(*col)->inweights[i]->_weight+=(*col)->learning_rate*(((*col)->inweights[i]->_positive_phase)-((*col)->inweights[i]->_negative_phase));
				cout<<endl<< "negative phase ="<<(*col)->inweights[i]->_negative_phase;
				cout<<endl<<"new weight ="<<(*col)->inweights[i]->_weight;
			}
		}

	}
}

void net::Output()
{
	int i=0;
	for(rrow=layers.rbegin(); rrow!=layers.rend(); rrow++)
	{
		for(rcol=rrow->rbegin(); rcol !=rrow->rend(); rcol++)
		{
			cout<<endl<<"neuron"<<i<<"="<<(*rcol)->_value;
			i++;
		}

	}
}

double net::Output(int row, int column=0)
{
	return layers[row][column]->_value;
}



void net::TrainRestricedBoltzman(int starting_layer)
{
		FeedForeward(starting_layer, starting_layer+1);

		for(col=(layers.at(starting_layer+1)).begin(); col !=((layers.at(starting_layer+1)).end()); col++)
	{
		threads.push_back(thread(&neuron::reconstruct,(*col)));
	}

	for(int i=0; i<threads.size(); i++)
	{
		threads[i].join();

	}

	threads.erase(threads.begin(),threads.end());//destroys threads in array to leave it empty for next layer

 int i=0;	for(col=(layers.at(starting_layer)).begin(); col !=(layers.at(starting_layer)).end(); col++)
	{
		i++;
		(*col)->activate();
		(*col)->_value=(*col)->_buffer;
		cout<<endl<<"neuron "<<i<<"="<<(*col)->_value;



	}


}


void net::OutputToVector(vector<vector<double>>& dataout)//dataout can only be a square
{
    int i=0;
    int j=0;
    for(col=(layers.at(layers.size()-1).begin()); col !=(layers.at(layers.size()-1).end()); col++)
    {
        if(i==dataout.size())break;
        dataout[i][j]=(*col)->_value;
        if(j==dataout[0].size()-1)
        {
            j=0;
            i++;
        }
        else{
            j++;
        }
    }

}



void net::OutputToVector(vector<double>& dataout)
{
    int i=0;
    for(col=(layers.at(layers.size()-1).begin()); col !=(layers.at(layers.size()-1).end()); col++)
    {
        if(i==dataout.size())break;
        dataout[i]=(*col)->_value;

        i++;
    }

}

net* clone(net& tobecloned)
{
    int row;
    int column;

    net* cloned_net = new net;
    cloned_net->layers.resize(tobecloned.layers.size());
    for (int i=0; i<tobecloned.layers.size()-1; i++)
    {
        for(int j=0; j<tobecloned.layers[i].size(); j++)
        {

            tobecloned.layers[i][j]->_buffer=i;
            tobecloned.layers[i][j]->_ebuffer=j;
            cloned_net->layers[i].push_back(new neuron);

        for(int k=0; k<tobecloned.layers[i][j]->inweights.size(); k++)
        {
            row=tobecloned.layers[i][j]->inweights[k]->_origin._buffer;
            column=tobecloned.layers[i][j]->inweights[k]->_origin._ebuffer;
            cloned_net->layers[i][j]->inweights.push_back(new link(*(cloned_net->layers[row][column])));
        }

        }

    }
    return cloned_net;
}

void net::AddNeurons(int amount, int layer)
{
    for (int i=0; i<amount; i++)
    {
        layers.at(layer).push_back(new neuron);
    }
}


void net::AddFullyConnectedNuerons(int amount, int layer)
{
	int original=layers.at(layer).size();

	for(int i=0; i<amount; i++)
	{
		layers.at(layer).push_back(new neuron);


		if (layer>0)
		{
		for (int j=0; j <layers [layer-1].size();j++)
		{

				layers[layer][original+i]->inweights.push_back (new link(*(layers [layer-1][j])));
				cout<<endl<<"new link from "<<(layer-1)<<","<<j<<" to "<<layer<<","<<original+i;

		}
		}

		if (layer<layers.size ()-1)
		{
		for (int k=0; k <layers [layer+1].size();k++)
		{
			layers[layer+1][k]->inweights.push_back (new link(*(layers [layer][original+i])));
			cout<<endl<<"new link from "<<(layer)<<","<<original+i<<" to "<<layer+1<<","<<k;
		}

	}
	}

}



void net:: Connect(int layer_from, int column_from, int layer_to, int column_to)
{
    layers.at(layer_to).at(column_to)->inweights.push_back(new link(*(layers[layer_from][column_from])));
    cout<<endl<<"new link from "<<layer_from<<":"<<column_from<<" to "<<layer_to<<":"<<column_to;
}

void net::FullyConnectNets(net* net_from, net* net_to, int layer_from, int layer_to)
{
    for(int i=0; i<net_to->layers.at(layer_to).size(); i++)
    {
        for(int j= 0; j<net_from->layers.at(layer_from).size();j++)
        {
            net_to->layers.at(layer_to).at(i)->inweights.push_back(new link(*net_from->layers.at(layer_from).at(j)));
        }

    }


}


void net::FullyConnect()
{
    for (int i=0; i <layers.size(); i++)
		{
			for (int j=0; j <layers[i].size(); j++)
			{

				if (i>=1)
					{
						for (int k=0; k <layers [i-1].size();k++)
						{

							layers[i][j]->inweights.push_back (new link(*(layers [i-1][k])));
							cout<<endl<<"new link from "<<(i-1)<<","<<k<<" to "<<i<<","<<j;

						}
					}
			}
		}

}


void net::FullyConnect(int layer_from, int layer_to)
{

    for (int i=layer_from; i <layer_to&&layers.size(); i++)
		{
			for (int j=0; j <layers[i].size(); j++)
			{

				if (i>=1)
					{
						for (int k=0; k <layers [i-1].size();k++)
						{

							layers[i][j]->inweights.push_back (new link(*(layers [i-1][k])));
							cout<<endl<<"new link from "<<(i-1)<<","<<k<<" to "<<i<<","<<j;

						}
					}
			}
		}
}

void net::SingleConnect()
{
    for (int i =0; i<layers.size(); i++)
    {
        for(int j=0; j<layers[i].size(); j++)
        {
            Connect((i-1),j,i,j);
        }
    }
}



void net::SingleConnect(int layer_from, int layer_to)
{
    for (int i =layer_from; i<layer_to; i++)
    {
        for(int j=0; j<layers[i].size(); j++)
        {
            Connect((i-1),(j-1),i,j);
        }
    }
}



