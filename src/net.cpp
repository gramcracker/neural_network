#include <iostream>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <thread>
#include "net.h"
#include "neuron.h"
#include <tuple>
#include <string>

using namespace std;

/**
 * @brief      Constructs the net object.
 */
net::net(){cout<<"new net"<<endl;};


/**
 * @brief      Constructs the net object.
 *
 * @param      dimensions  The dimensions of the network where the input layer is dimensions[0] and the output is dimensions.back()
 */
net::net(vector <int> &dimensions)
{
    //creates a number of neurons and layers based on the dimensions
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


/**
 * @brief      Constructs the net object.
 *
 * @param      dimensions The dimensions of the network where the input layer is dimensions[0] and the output is dimensions.back()     
 * @param[in]  fullyconnected  fullyconnected = true creates complete connections between each layer 
 */
net::net(vector <int> &dimensions, bool fullyconnected)
{

    //creates a number of fully connected neurons and layers based on the dimensions
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
                            //cout<<endl<<"new link from "<<(i-1)<<","<<k<<" to "<<i<<","<<j;

                        }
                    }
            }
        }
};

/**
 * @brief      Destroys the net along with all of its pointers to neuron objects.
 */
net::~net()
    {
        if(this->netPointersCopied){
            cout <<endl<<"netPointersCopied";
            return;
        }
        //deletes all neurons and their inweights
        for(row=layers.begin(); row!=layers.end(); row++)
        {

            for(col=row->begin(); col !=row->end(); col++)
            {
                delete(*col); //deletes the neuron

            }
            row->clear();//clear the row it points too
        }
    layers.clear();

    }


/**
 * @brief      Constructs the object.
 *
 * @param[in]  tobecloned  The tobecloned
 */
net::net(const net& tobecloned)
{

    int y;
    int x;

    for(row=layers.begin(); row!=layers.end(); row++)
    {

        for(col=row->begin(); col !=row->end(); col++)
        {

            delete(*col); //deletes the node
        }
        row->clear();//clear the row it points too
    }
    layers.clear();


    layers.resize(tobecloned.layers.size());
    for (int i=0; i<tobecloned.layers.size(); i++)
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


// overloaded assignment operator for the net class
net& net::operator=(const net& tobecloned)
    {
        int y;
        int x;


        for(row=layers.begin(); row!=layers.end(); row++)
        {

            for(col=row->begin(); col !=row->end(); col++)
            {


                delete(*col); //deletes the node
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




//return the amount of layers
int net::get_size()
{
    return layers.size();
}

//returns the amount of neurons in a specific layer
int net::get_size(int i)
{

    return layers[i].size();
}


//sets the input to the net
void net::SetInput(vector<double>& input)
{
    for( int i=0; i<layers.at(0).size(); i++)
    {
        layers.at(0).at(i)->_input=(input[i]);
    };
}

void net::UserInput()
{
    for( int i=0; i<layers.at(0).size(); i++)
    {
        cout<<endl<<"Input value for neuron("<<i<<") ";
        double d;
        cin>>d;
        layers.at(0).at(i)->_input=(d);
    };
}


//adds functions from the neuron class to the Sequence to be run
void net::AddToSequence(int iteration, int layer, int first, int last, void(neuron::*func)())
{

    //NOTE: THIS FUNCTION WILL ADD NEURONS TO THE NET IF THEY DON'T EXSIST YET
    //switch first and last to correct the order
    if(first>last)
    {
        cerr<<"Attempted invalid sequence order in function net::AddToSequence(), int first and int last will be switched.";
        int temp=first;
        first=last;
        last=temp;
    }


 //////match up network--------------------------
    //check for matching number of layers
    if(layers.size()<layer+1)
    {

        layers.resize(layer+1);
    }
    //check for matching number of neurons in layer
    if(layers.at(layer).size()<last+1)
    {

        AddNeurons(((last+1)-layers.at(layer).size()),layer);
    }


///////match up Sequence-----------------------------------
    //resize sequence to correct number of iterations

    if(Sequence.size()<iteration+1)
    {
       Sequence.resize(iteration+1);
    }


     for(int i=first; i<last+1; i++)
        {
            Sequence.at(iteration).push_back(new Operation(layer,i, func));
            //Sequence.at(iteration)[i-first]->NeuronFunction=(make_tuple(layer,i, func));//push function and neuron location pointer to Sequence
        }

}





void net:: RunSequence()
{
    vector<thread>threads;
    int row=0;
    int col=0;
    for(int i=0;i<Sequence.size();i++)
    {
        for(int j=0;j<Sequence[i].size();j++)
        {
            if(Sequence[i][j]->isnetfunc==true)
            {
                threads.push_back(thread( Sequence[i][j]->NetFunction));//function from this networks scope
            }
            else{
                row=Sequence[i][j]->row;
                col=Sequence[i][j]->col;
                threads.push_back(thread( Sequence[i][j]->NeuronFunction, layers[row][col]) );//function from a neurons scope
            }
        }


                //execute threads on layer
            for(int i=0; i<threads.size(); i++)
            {
                threads[i].join();
            }

            //clean up thread array
            threads.erase(threads.begin(),threads.end());

            if (threads.size()>0){cout<<endl<<threads.size()<<"threads remain";}
    }
}



int j=0;

void net::SetDesired(vector<double>& desired)
{
    for( int x=0; x<layers.at(layers.size()-1).size(); x++)
    {
        layers.at(layers.size ()-1).at(x)->_desired=(desired[x]);
    };
}





void net::FeedForeward(neuron::ActivationType activation_type=neuron::fast_sigmoid)
{

    vector<thread> threads;
    for(int i=0; i<layers.size(); i++)
    {


        for(int j=0; j<layers[i].size(); j++)
        {

            threads.push_back(thread([=]{(layers[i][j])->feed(activation_type);}));
        }
        for(int k=0; k<threads.size(); k++)
        {
        
            threads[k].join();
        }

        threads.erase(threads.begin(),threads.end());

        if (threads.size()>0){cout<<endl<<threads.size()<<"threads remain";}

        for(int l=0; l<layers[i].size(); l++)
        {
            (((layers[i][l])->_value)=(layers[i][l])->_buffer);//to avoid accessing and activating a neuron at the same time ( like if a neuron is connected to one in the same layer)
        }

    }

}




void net::FeedForeward( int starting_layer, int ending_layer, neuron::ActivationType activation_type=neuron::fast_sigmoid)
{
    vector<thread> threads;

    for(row=(layers.begin()+starting_layer); row!=(layers.begin()+ending_layer+1); row++)
    {
        for(col=row->begin(); col !=row->end(); col++)
        {
            threads.push_back(thread([=]{(*col)->feed(activation_type);}));//using this instead to allow overloading feed()
            //threads.push_back(thread(&neuron::feed,(*col)));
        }
        for(int i=0; i<threads.size(); i++)
        {
            threads[i].join();
        }

        threads.erase(threads.begin(),threads.end());

        if (threads.size()>0){cout<<endl<<threads.size()<<"threads remain";}

        for(col=row->begin(); col !=row->end(); col++)
        {
            ((*col)->_value)=((*col)->_buffer);//to avoid accessing and activating a neuron at the same t[o98ume ( like if a neuron is connected to one in the same layer)
        }

    }

}




void net::FeedForeward( int starting_layer, int ending_layer, int first_neuron, int last_neuron, neuron::ActivationType activation_type=neuron::fast_sigmoid)
{
    vector<thread> threads;
    for(int i=starting_layer; i!=ending_layer; i++)
        {
            for(int j=first_neuron; j!=last_neuron; j++)
            {
                 threads.push_back(thread([=]{layers[i][j]->feed(activation_type);}));

                if(j==layers[i].size()-1)
                {
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


        }



}



void net::Backpropagate()
{

    vector<thread> threads;

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




void net::TrainBackpropagation(vector<double>& _desired, vector <double>& _input)
{

    SetDesired(_desired);   //sets the desired Output of the network for Training
    SetInput(_input);
    FeedForeward();
    Backpropagate();

}

void net::TrainBackpropagation(vector<double>& _desired, vector <double>& _input, int _iterations){
    for(int i = 0; i<=_iterations; i++){
        TrainBackpropagation(_desired, _input);
    }
}


void net::ContrastDiverge( int starting_layer)
{
    vector<thread> threads;

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



void net::TrainRestrictedBoltzman(int starting_layer)
{
        vector<thread> threads;
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

 int i=0;   for(col=(layers.at(starting_layer)).begin(); col !=(layers.at(starting_layer)).end(); col++)
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
    for (int i=0; i<tobecloned.layers.size(); i++)
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
    if (layers.empty())
    {
        layers.resize(1);
    }

    if(layers.size()<layer)
    {
        cout<<"Unable to append neurons at layer "<<layer<<". Layer value must be less than or equal to "<<layers.size()<<" to build layers sequentially";
        return;

    }else if(layers.size()==layer)
    {
        vector <neuron*> l;
        layers.push_back(l);
    }

    for (int i=0; i<amount; i++)
    {
        layers[layer].push_back(new neuron);
    }
}




void net::AddNeurons(int amount, int layer, string identifying_tag)
{


    if (layers.empty())
    {
        layers.resize(1);
    }

    if(layers.size()<layer)
    {
        cout<<"Unable to append neurons at layer "<<layer<<". Layer value must be less than or equal to "<<layers.size()<<" to build layers sequentially";
        return;

    }else if(layers.size()==layer)
    {
        vector <neuron*> l;
        layers.push_back(l);
        cout<<"layers size: "<<layers.size()<<endl;
    }

    if(groups.size()<layers.size())groups.resize(layers.size());//
    if(groups[layer].size()<layers[layer].size())groups[layer].resize(layers[layer].size());

    for (int i=0; i<amount; i++)
    {
        layers[layer].push_back(new neuron);
        groups[layer].push_back(identifying_tag);
        //cout <<"new neuron at "<<layer<<" in group "<<identifying_tag<<endl;

    }

}


void net::AddFullyConnectedNeurons(int amount, int layer)
{
    if(layers.size()-1<layer){
        if(layers.size()-1<layer-1){cerr<<"ERROR: position of new layer is not within last layer position + 1";
        return;
    }
            else{layers.resize(layer+1);}
                
        
    }

    int original=layers.at(layer).size();

    for(int i=0; i<amount; i++)
    {
        layers.at(layer).push_back(new neuron);


        if (layer>0)
        {
        for (int j=0; j <layers [layer-1].size();j++)
        {

                layers[layer][original+i]->inweights.push_back (new link(*(layers [layer-1][j])));
                //cout<<"new link from "<<(layer-1)<<","<<j<<" to "<<layer<<","<<original+i<<endl;

        }
        }

        if (layer<layers.size ()-1)
        {
        for (int k=0; k <layers [layer+1].size();k++)
        {
            layers[layer+1][k]->inweights.push_back (new link(*(layers [layer][original+i])));
            //cout<<"new link from "<<(layer)<<","<<original+i<<" to "<<layer+1<<","<<k<<endl;
        }

    }
    }

}



void net:: Connect(int layer_from, int column_from, int layer_to, int column_to)
{
    layers.at(layer_to).at(column_to)->inweights.push_back(new link(*(layers.at(layer_from).at(column_from))));
    cout<<"new link from "<<layer_from<<":"<<column_from<<" to "<<layer_to<<":"<<column_to<<endl;
}



void net:: Connect(int layer_from, int column_from, int layer_to, int column_to, float weight)
{
    layers.at(layer_to).at(column_to)->inweights.push_back(new link((*(layers.at(layer_from).at(column_from))), weight));
    //cout<<"new link from "<<layer_from<<":"<<column_from<<" to "<<layer_to<<":"<<column_to<<endl;
}




void net::FullyConnectToNet( net* net_to, int layer_from, int layer_to)
{
    for(int i=0; i<net_to->layers.at(layer_to).size(); i++)
    {
        for(int j= 0; j<this->layers.at(layer_from).size();j++)
        {
            net_to->layers.at(layer_to).at(i)->inweights.push_back(new link(*this->layers.at(layer_from).at(j)));
        }

    }

}

net* net::AppendNet(net* net_from, bool pasteOver)
{
    if(pasteOver){
            vector<vector<neuron*>> l = net_from->layers; 
            this->layers.resize(0);
            this->layers.insert(this->layers.end(), l.begin(), l.end());


    }else{
        this->layers.insert(this->layers.end(), net_from->layers.begin(), net_from->layers.end());
    }
    
    net_from->netPointersCopied = true;
    return this;
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
                            //cout<<endl<<"new link from "<<(i-1)<<","<<k<<" to "<<i<<","<<j;

                        }
                    }
            }
        }

}


void net::FullyConnect(int layer_from, int layer_to)
{
    if(layer_from==layer_to)
    {
        for (int j=0; j <layers[layer_from].size(); j++)
        {
            for (int k=0; k <layers[layer_to].size();k++)
                {

                    layers[layer_to][j]->inweights.push_back (new link(*(layers [layer_from][k])));
                    //cout<<endl<<"new link from "<<layer_from<<","<<j<<" to "<<layer_to<<","<<k;

                }
        }
    }else{

        for (int i=layer_from; i <=layer_to && i<layers.size(); i++)
        {

            for (int j=0; j <layers[i].size(); j++)
            {

                if (i>=layer_from+1)
                    {
                        for (int k=0; k <layers [i-1].size();k++)
                        {

                            layers[i][j]->inweights.push_back (new link(*(layers [i-1][k])));
                            //cout<<endl<<"new link from "<<(i-1)<<","<<k<<" to "<<i<<","<<j;

                        }
                    }
            }
        }
    }


}

void net::FullyConnect(int layer_from, int lfstart, int lfend, int layer_to, int ltstart, int ltend)
{
    if(layer_from==layer_to)
    {
        for (int j=lfstart; j <=lfend; j++)
        {
            for (int k=ltstart; k <=ltend;k++)
                {

                    layers[layer_to][j]->inweights.push_back (new link(*(layers [layer_from][k])));
                    //cout<<endl<<"new link from "<<layer_from<<","<<j<<" to "<<layer_to<<","<<k;

                }
        }
    }else{

        for (int i=layer_from; i <=layer_to && i<layers.size(); i++)
            {

                cout<<endl<<"i="<<i;

                for (int j=ltstart; j <=(ltend-ltstart); j++)
                {
                    cout<<endl<<"j="<<j;

                    if (i>=(layer_from+1))
                        {
                            for (int k=lfstart; k <(lfend-lfstart);k++)
                            {

                                layers[i][j]->inweights.push_back (new link(*(layers [i-1][k])));
                                //cout<<endl<<"new link from "<<(i-1)<<","<<k<<" to "<<i<<","<<j;

                            }
                        }
                }
            }
        }
}

void net::SingleConnect()
{
    for (int i =1; i<layers.size(); i++)
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
            Connect(layer_from,j,layer_to,j);
            //cout<<"new link from "<<layer_from<<","<<j<<" to "<<layer_to<<","<<j<<endl;
        }
    }
}

void net::SingleConnect( string identifying_tag_1, string identifying_tag_2)
{
    //this for loop iterates through groups and matches the tags^ to groups and saves the coordinates of the matching neurons in the place holder
    vector <vector <int>> place_holder(4);
    for(int i=0; i<groups.size(); i++)
    {
        for(int j=0; j<groups[i].size(); j++)
        {
            if((groups[i][j])==identifying_tag_1)
            {
                place_holder[0].push_back(i);
                place_holder[1].push_back(j);

            }
            if((groups[i][j])==identifying_tag_2)
            {
                place_holder[2].push_back(i);
                place_holder[3].push_back(j);

            }
        }
    }

    if(place_holder[0].size()>place_holder[2].size())
    {
        cout<<"Unable to single connect group: "<<identifying_tag_1<<" to group: "<<identifying_tag_2<<". "<<identifying_tag_2<<" must have at least "<<place_holder[0].size()<<" neurons.";
        return;
    }

    //this is where the actual onnecting takes place
    for(int i=0; i<place_holder[0].size(); i++)
    {
        Connect(place_holder[0][i],
                place_holder[1][i],
                place_holder[2][i],
                place_holder[3][i]);
    }
        if(place_holder[0].size()<place_holder[2].size())
    {
        cout<<"Warning, "<<place_holder[2].size()-place_holder[0].size()<<" neurons left unconnected in group: "<<identifying_tag_2<<" due to unequal quantities between the two groups";
    }
}

void net::SingleConnectToColumns(int layer_from, int layer_to)
{
    int rows=layers[layer_from].size();
    int columns= (layers[layer_to].size())/rows;
    if ((((float)layers[layer_to].size())/(float)rows)==columns)
    {


        for (int i =0; i<rows; i++)
        {
            for(int j=columns*i; j<(columns*(i+1));j++)
            {
                Connect(layer_from,i,layer_to,j);
            }
        }
    }else{
            cout<<endl<<"Layer "<<layer_from<<" could not be singly connected to rows of layer "<<layer_to<<".";
        }


}

void net::SingleConnectToColumns(string identifying_tag_1, string identifying_tag_2)
{

    //this for loop iterates through groups and matches the tags^ to groups and saves the coordinates of the matching neurons in the place holder
    vector <vector <int>> place_holder(4);
    for(int i=0; i<groups.size(); i++)
    {
        for(int j=0; j<groups[i].size(); j++)
        {
            if((groups[i][j])==identifying_tag_1)
            {
                place_holder[0].push_back(i);
                place_holder[1].push_back(j);

            }
            if((groups[i][j])==identifying_tag_2)
            {
                place_holder[2].push_back(i);
                place_holder[3].push_back(j);

            }
        }
    }

    int rows=place_holder[0].size();
    int columns= (place_holder[2].size())/rows;
    if ((((float)place_holder[2].size())/(float)rows)==columns)
    {


        for (int i =0; i<rows; i++)
        {
            for(int j=columns*i; j<(columns*(i+1));j++)
            {
                Connect(place_holder[0][i],place_holder[1][i],place_holder[2][j],place_holder[3][j]);
            }
        }
    }else{
            cout<<endl<<"Group "<<identifying_tag_1<<" could not be singly connected to rows of group "<<identifying_tag_2<<".";
        }
}



void net::SingleConnectToRows(int layer_from, int layer_to)
{
    int columns=layers[layer_from].size();
    int rows=(layers[layer_to].size())/columns;
    int k=0;
    if ((((float)layers[layer_to].size())/(float)columns)==rows)
    {
        for (int i =0; i<rows; i++)
        {
            for(int j=0; j<columns;j++,k++)
            {
                Connect(layer_from,j,layer_to,k);

            }
        }
    }else{
            cout<<endl<<"Layer "<<layer_from<<" could not be singly connected to columns of layer "<<layer_to<<".";
        }


}


void net::SingleConnectToRows(string identifying_tag_1, string identifying_tag_2)
{
    vector <vector <int>> place_holder(4);
    for(int i=0; i<groups.size(); i++)
    {
        for(int j=0; j<groups[i].size(); j++)
        {
            if((groups[i][j])==identifying_tag_1)
            {
                place_holder[0].push_back(i);
                place_holder[1].push_back(j);

            }
            if((groups[i][j])==identifying_tag_2)
            {
                place_holder[2].push_back(i);
                place_holder[3].push_back(j);

            }
        }
    }

    int columns=place_holder[0].size();
    int rows= (place_holder[2].size())/columns;
    int k=0;
    if ((((float)place_holder[2].size())/(float)columns)==rows)
    {
        for (int i =0; i<rows; i++)
        {
            for(int j=0; j<columns;j++,k++)
            {
                Connect(place_holder[0][j],place_holder[1][j],place_holder[2][k],place_holder[3][k]);

            }
        }
    }else{
            cout<<endl<<"Group "<<identifying_tag_1<<" could not be singly connected to columns of group "<<identifying_tag_2<<".";
        }


}


void net::SingleConnectFromColumns(int layer_from, int layer_to)
{
    int rows=layers[layer_to].size();
    int columns= (layers[layer_from].size())/rows;
    if ((((float)layers[layer_from].size())/(float)rows)==columns)
    {


        for (int i =0; i<rows; i++)
        {
            for(int j=columns*i; j<(columns*(i+1));j++)
            {
                Connect(layer_from,j,layer_to,i);
            }
        }
    }else{
            cout<<endl<<"Layer "<<layer_from<<" could not be singly connected to rows of layer "<<layer_to<<".";
        }


}

void net::SingleConnectFromRows(int layer_from, int layer_to)
{
    int columns=layers[layer_to].size();
    int rows=(layers[layer_from].size())/columns;
    int k=0;
    if ((((float)layers[layer_from].size())/(float)columns)==rows)
    {
        for (int i =0; i<rows; i++)
        {
            for(int j=0; j<columns;j++,k++)
            {
                Connect(layer_from,k,layer_to,j);
            }
        }
    }else{
            cout<<endl<<"Layer "<<layer_from<<" could not be singly connected to columns of layer "<<layer_to<<".";
        }


}




void net::Save(ofstream& file, bool save_neuron_values)
{
    if(!file.is_open())cout<<"File could not be opened. "<<endl;

    int link_count=0;
    vector <tuple<int,int,int,double>> linkdata;
    if(save_neuron_values==true)
    {
        file<<'R'<<endl;//R for recall
    }else{
        file<<'F'<<endl;//F for forget
    }

    file<<layers.size()<<" ";//first number is the number of layers


    ////first loop is to place all neurons( we also have to set coordinates of each neuron first(to allow recurrent connections when opening the file) )
    for (int i=0; i<layers.size(); i++)
    {
        file<<layers[i].size()<<" ";//the fist number for each layer is the number of neurons

        for(int j=0; j<layers[i].size(); j++)
        {
            //set the coordinates inside each neuron to access them later
            layers[i][j]->row=i;
            layers[i][j]->col=j;
            link_count+=layers[i][j]->inweights.size();

            if(save_neuron_values==true)
            {
                file<<layers[i][j]->_value<<" ";//in this mode the vales of neurons are saved
            }

        }

    }
    file<<link_count<<" ";
 ////second loop is to connect all neurons(we can get coordinates of neurons now)////
    for (int i=0; i<layers.size(); i++)
    {

        for(int j=0; j<layers[i].size(); j++)
        {

            for(int k=0; k<layers[i][j]->inweights.size(); k++)//then 5 values representing connection coordinates and weight value
            {
                file<<layers[i][j]->inweights[k]->_origin.row<<" ";//layer to connect from
                file<<layers[i][j]->inweights[k]->_origin.col<<" ";//neuron to connect from
                file<<i<<" ";//layer to connect to
                file<<j<<" ";//neuron to connect to
                file<<layers[i][j]->inweights[k]->_weight<<" ";//weight(double)
            }
        }
    }
    file.close();
}


void net::Open(ifstream& file)
{

    if(!file.is_open())
    {
        cout<<"Network file could not be opened."<<endl;
        return;
    }
    else{cout<<"Now writing network to file."<<endl;}

    bool VALUES_SAVED = false;
    char state='F';
    int layer_count=0;
    int neuron_count=0;
    int link_count=0;
    int lf,nf,lt,nt;
    float wval;

    file>>state;
    if(state=='R')
    {
        VALUES_SAVED=true;
    }else if(state=='F')
    {
        VALUES_SAVED=false;
    }
    else{
        cerr<<"Error: file must start with either R (remember) or F( forget) to specify if the neurons states where stored"<<endl;
    }

    file>>layer_count;
    cout<<"layer count: "<<layer_count<<endl;
    layers.resize(layer_count);

    for (int i=0; i<layer_count; i++)
    {
        file>>neuron_count;
        cout<<"neuron count: "<<neuron_count<<endl;

        for(int j=0; j<neuron_count; j++)
            {
                layers[i].push_back(new neuron);

                if(VALUES_SAVED==true)
                {
                    file>>(layers[i][j]->_value);
                }
            }
    }

    file>> link_count;
    cout<<"link count: "<<link_count<<endl;

    for (int i=0; i<link_count; i++)
    {

        file>>lf;//layer from
        file>>nf;//neuron from
        file>>lt;//layer to
        file>>nt;//neuron to
        cout<<"connecting : "<<lt<<","<<nt<<" from "<<lf<<","<<nf<<endl;
        file>>wval;
        cout<<"weight value :"<<wval<<endl;
        Connect(lf,nf,lt,nt,wval);


    }
    file.close();
}










void net::SetWeights(double val, int _layer, int _neuron, int _link)
{
    layers.at(_layer).at(_neuron)->inweights.at(_link)->SetWeight(val);
}

void net::SetWeights(double val, int _layer, int _neuron)
{
    for(int i=0; i<layers.at(_layer).at(_neuron)->inweights.size(); i++)
    {
        SetWeights(val, _layer, _neuron, i);
    }

}

void net::SetWeights(double val, int _layer)
{
    for(int i=0; i<layers.at(_layer).size(); i++)
    {
        SetWeights(val, _layer, i);
    }

}

void net::SetWeights(double val)
{
     for(int i=0; i<layers.size(); i++)
     {
         SetWeights(val, i);
     }

}









void net::CleanAfterBuild()
{
    cout<<"Finished building net. Deleting extra memory used in build process."<<endl;
    
     groups.resize(0);

}









void net::RandomizeWeights(int _layer, int _neuron, int _link)
{
 layers.at(_layer).at(_neuron)->inweights.at(_link)->SetRandWeight();
}

void net::RandomizeWeights(int _layer, int _neuron)
{
    for(int i=0; i<layers.at(_layer).at(_neuron)->inweights.size(); i++)
    {
        RandomizeWeights(_layer, _neuron, i);
    }

}

void net::RandomizeWeights(int _layer)
{
    for(int i=0; i<layers.at(_layer).size(); i++)
    {
        RandomizeWeights(_layer, i);
    }

}

void net::RandomizeWeights()
{
    for(int i=0; i<layers.size(); i++)
    {
       RandomizeWeights(i);
    }

}


void net::SetBias(double val, int _layer, int _neuron)
{
    layers.at(_layer).at(_neuron)->set_bias(val);
}
void net::SetBias(double val, int _layer)
{
    for(int i=0; i<layers.at(_layer).size(); i++)
    {
        SetBias(val, _layer, i);
    }
}
void net::SetBias(double val)
{
    for(int i=0; i<layers.size(); i++)
    {
        SetBias(val, i);
    }
}
