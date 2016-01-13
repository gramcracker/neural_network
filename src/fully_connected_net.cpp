#include "fully_connected_net.h"
#include <iostream>



fully_connected_net::fully_connected_net( vector<int> &dimensions)
	{
		layers.resize(dimensions.size());
		cout<<endl<<"size = "<<dimensions.size();
		for (int i=0; i <dimensions.size(); i++)
		{
			for (int j=0; j <dimensions[i]; j++)
			{
				layers.at(i).push_back(new neuron);

				cout<<endl<<"new neuron at "<<i<<","<<j;
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






fully_connected_net::~fully_connected_net()
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



void fully_connected_net::AddNeurons(int amount, int layer)
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
