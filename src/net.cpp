#include <iostream>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <thread>
#include "net.h"
#include "neuron.h"
#include <tuple>
#include <string>
#include <unordered_set>


using namespace std;



/**
 * @brief      Constructs the net object.
 */
net::net(){


    cout<<"new net"<<endl;
    
};


/**
 * @brief      Constructs the net object.
 *
 * @param      dimensions The dimensions of the network where the input layer is dimensions[0] and the output is dimensions.back()     
 * @param[in]  fully_connected  fully_connected = true creates complete connections between each layer 
 */
net::net(vector <int> &dimensions, bool fully_connected)
{

    //creates a number of fully connected neurons and layers based on the dimensions
    int column_size = dimensions.size();
    layers.resize(column_size);

    if (fully_connected==true)
    {
        for(int i = 0; i<column_size; i++){
            AddFullyConnectedNeurons(dimensions[i], i);
        }
    }else{
        AddNeurons(dimensions);
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
        //deletes all neurons and their outweights
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

            for(int k=0; k<tobecloned.layers[i][j]->outweights.size(); k++)
            {
                y=tobecloned.layers[i][j]->outweights[k]->postsynaptic._buffer;
                x=tobecloned.layers[i][j]->outweights[k]->postsynaptic._ebuffer;
                layers[i][j]->outweights.push_back(new link(*(layers[y][x])));
            }

        }

    }
}



/**
 * @brief      { reshapes a net to have the same shape as the net passed in }
 *
 * @param[in]  tobecloned  The net with the shape to be copied
 *
 * @return     { *this }
 */
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

                for(int k=0; k<tobecloned.layers[i][j]->outweights.size(); k++)
                {
                    y=tobecloned.layers[i][j]->outweights[k]->postsynaptic._buffer;
                    x=tobecloned.layers[i][j]->outweights[k]->postsynaptic._ebuffer;
                    layers[i][j]->outweights.push_back(new link(*(layers[y][x])));
                }

            }

        }

        return (*this);

    }

/**
 * @brief Sets the verbose mode flag to generate verbose output log to terminal.
 * 
 * @param verboseMode 
 */
void net::SetVerboseMode(bool verboseMode){
    VERBOSE_MODE = verboseMode;
}


/**
 * @brief      Gets the number of layers.
 *
 * @return     The size.
 */
int net::get_size()
{
    return layers.size();
}

/**
 * @brief      Gets the size of a specific layer.
 *
 * @param[in]  i     { layer number }
 *
 * @return     The size/amount of neurons in layer.
 */
int net::get_size(int i)
{

    return layers[i].size();
}

/**
 * @brief      Sets the input layer from data stored in a vector.
 *
 * @param      input  The input data
 */
void net::SetInput(vector<double>& input)
{
    for( int i=0; i<layers.at(0).size(); i++)
    {
        layers.at(0).at(i)->setin(input[i]);
    };
}

/**
 * @brief      Adds functions from the neuron class to the Sequence to be run.
 *
 * @param[in]  iteration  The iteration in Sequence that triggers this function
 * @param[in]  layer      The layer  with the neurons that the function will will be called from
 * @param[in]  first      The first neuron in the part of the layer that the function will be called from
 * @param[in]  last       The last neuron in the part of the layer that the function will be called from
 * @param[in]  <unnamed>  { parameter_description }
 */
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




/**
 * @brief      { Runs the sequence that operates the network }
 */
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

/**
 * @brief      Sets the desired output for the network.
 *
 * @param      desired  The desired output for the network
 */
void net::SetDesired(vector<double>& desired)
{
    for( int x=0; x<layers.at(layers.size()-1).size(); x++)
    {
        layers.at(layers.size ()-1).at(x)->_desired=(desired[x]);
    };
}





// void net::FeedForeward(neuron::ActivationType activation_type)
// {
//     neuron::ActivationType _activation_type = activation_type;
//     vector<thread> threads;
//     for(int i=0; i<layers.size(); i++)
//     {

//          if (i==0)_activation_type = neuron::none;//if input layer, dont activate, just multiply by weights
//          else{
//      _activation_type = activation_type;
//          }

//         for(int j=0; j<layers[i].size(); j++)
//         {
//             threads.push_back(thread([=]{(layers[i][j])->feed(_activation_type);}));
//         }
//         for(int k=0; k<threads.size(); k++)
//         {
//             threads[k].join();
//         }

//         threads.erase(threads.begin(),threads.end());

//         if (threads.size()>0){cout<<endl<<threads.size()<<"threads remain";}

//     }

// }

void net::FeedForeward(neuron::ActivationType activation_type){
    
    vector<thread> threads;

    unordered_set <neuron*> presynapticSubset;
    unordered_set <neuron*> postsynapticSubset;
    
    for (auto& n : layers[0]){
        if(!n->_buffer==0){
            threads.push_back(thread(&neuron::ffeed, n, neuron::none, std::ref(postsynapticSubset))); //active l0 set post
        }
    }
    start:
    for(int i = 0; i< threads.size(); i++){threads[i].join();}                              //join threads
    threads.erase(threads.begin(),threads.end());                                           //erase threads
    presynapticSubset.erase(presynapticSubset.begin(), presynapticSubset.end());            //clear pre
    if(postsynapticSubset.empty())return;                                                     //check if post
    cout<<endl;

    for(auto &n : postsynapticSubset){
        threads.push_back(thread(&neuron::ffeed, n, activation_type, std::ref(presynapticSubset)));     //active post set pre
        
    }
    for(int i = 0; i< threads.size(); i++){threads[i].join();}                            //join threads
    threads.erase(threads.begin(),threads.end());                                           //erase threads
    postsynapticSubset.erase(postsynapticSubset.begin(), postsynapticSubset.end());         //clear post
    cout<<endl;

    if(presynapticSubset.empty())return;                                                  //check if pre
    for(auto& n: presynapticSubset){
        threads.push_back(thread(&neuron::ffeed, n, activation_type, std::ref(postsynapticSubset)));    //active pre set post
    
    }
    goto start;
}



//todo: fix to match above
void net::FeedForeward( int starting_layer, int ending_layer, neuron::ActivationType activation_type)
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




void net::FeedForeward( int starting_layer, int ending_layer, int first_neuron, int last_neuron, neuron::ActivationType activation_type)
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


            for(unsigned int i=0;i<((*col)->outweights.size());i++)
            {
                (*col)->outweights[i]->_positive_phase=((*col)->_value)*((*col)->outweights[i]->postsynaptic._value);
                cout<<endl<<"positive phase ="<<(*col)->outweights[i]->_positive_phase;
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


            for(unsigned int i=0;i<((*col)->outweights.size());i++)
            {

                (*col)->outweights[i]->_negative_phase=((*col)->_value)*((*col)->outweights[i]->postsynaptic._input);
                (*col)->outweights[i]->_weight+=(*col)->learning_rate*(((*col)->outweights[i]->_positive_phase)-((*col)->outweights[i]->_negative_phase));
                cout<<endl<< "negative phase ="<<(*col)->outweights[i]->_negative_phase;
                cout<<endl<<"new weight ="<<(*col)->outweights[i]->_weight;
            }
        }

    }
}


void net::Output()
{
    for(int i=0; i<layers.size(); i++)
    {
        for(int j=0; j<layers.at(i).size(); j++)
        {
            cout<<"("<<i<<","<<j<<")="<<layers.at(i).at(j)->_value<<" ";
        }
        cout<<endl;

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
        if(VERBOSE_MODE){
            cout<<endl<<"neuron "<<i<<"="<<(*col)->_value;
        }
        
    }
}


void net::TrainRestrictedBoltzman(int starting_layer, int iterations){
    for(int i=0; i<iterations; i++){
        TrainRestrictedBoltzman(starting_layer);
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

        for(int k=0; k<tobecloned.layers[i][j]->outweights.size(); k++)
        {
            row=tobecloned.layers[i][j]->outweights[k]->postsynaptic._buffer;
            column=tobecloned.layers[i][j]->outweights[k]->postsynaptic._ebuffer;
            cloned_net->layers[i][j]->outweights.push_back(new link(*(cloned_net->layers[row][column])));
        }

        }

    }
    return cloned_net;
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
        if(VERBOSE_MODE){
            cout<<"layers size: "<<layers.size()<<endl;
        }
    }

    if(identifying_tag!=""){
        if(groups.size()<layers.size()) groups.resize(layers.size());//
        if(groups[layer].size()<layers[layer].size()) groups[layer].resize(layers[layer].size());
    }
    

    for (int i=0; i<amount; i++)
    {
        layers[layer].push_back(new neuron);
        if(identifying_tag!="") groups[layer].push_back(identifying_tag);
        if(VERBOSE_MODE){
            cout <<"new neuron at "<<layer<<" in group "<<identifying_tag<<endl;
        }
        

    }

}

void net::AddNeurons(vector<int> &dimensions, string identifying_tag){
    for(int i=0; i<dimensions.size(); i++){
        AddNeurons(dimensions[i],i, identifying_tag);
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


void net:: Connect(int layer_from, int column_from, int layer_to, int column_to, float weight)
{
    layers.at(layer_from).at(column_from)->outweights.push_back(new link((*(layers.at(layer_to).at(column_to))), weight));
    if(VERBOSE_MODE){
        cout<<"new link from "<<layer_from<<":"<<column_from<<" to "<<layer_to<<":"<<column_to<<" val:"<<weight<<endl;
    }
    
}

void net::Connect(int layer_from, int column_from, int layer_to, int column_to)
{
    Connect(layer_from, column_from, layer_to, column_to, rand_generator.generate());
}


void net::FullyConnect( net* net_to, int layer_from, int layer_to)
{
    for(int i=0; i<net_to->layers.at(layer_to).size(); i++)
    {
        for(int j= 0; j<this->layers.at(layer_from).size();j++)
        {
            net_to->layers.at(layer_to).at(i)->outweights.push_back(new link(*this->layers.at(layer_from).at(j)));
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

                    Connect(layer_from, k, layer_to, j);

                }
        }
    }else{

        for (int i=layer_from; i <=layer_to && i<layers.size(); i++)
            {

                for (int j=ltstart; j <=(ltend-ltstart); j++)
                {

                    if (i>=(layer_from+1))
                        {
                            for (int k=lfstart; k <=(lfend-lfstart);k++)
                            {
                                
                                Connect(i-1,k,i,j);

                            }
                        }
                }
            }
        }
}

void net::FullyConnect(int layer_from, int layer_to)
{
    FullyConnect(layer_from, 0, layers[layer_from].size()-1, layer_to, 0, layers[layer_to].size()-1);
}

void net::FullyConnect()
{
    for (int i=0; i <layers.size()-1; i++)
        {
            FullyConnect(i,i+1);
        }

}

void net::AddFullyConnectedNeurons(int amount, int layer)
{
    if(layers.size()-1 < layer){
        if(layers.size()-1 < layer-1){
            cerr<<"ERROR: position of new layer is not within last layer position + 1";
        return;

        }else{
            layers.resize(layer+1);
        }
                
        
    }

    int original=layers.at(layer).size();
    //fully connect to any layers before this layer in this loop
    for(int i=0; i<amount; i++)
    {

        layers.at(layer).push_back(new neuron);

        if (layer>0)
        {
            for (int j=0; j < layers[layer-1].size();j++){

                    //layers[layer][original+i]->outweights.push_back (new link(*(layers [layer-1][j])));
                    Connect(layer-1, j, layer, original + i);


            }
        }

        //now fully connect to any layers that come after this layer in this loop
        if (layer < layers.size()-1)
        {
            for (int k=0; k <layers [layer+1].size();k++)
            {
                //layers[layer+1][k]->outweights.push_back (new link(*(layers [layer][original+i])));
                Connect(layer, original+i, layer+1, k);
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




ofstream& net::Save(ofstream& file, bool save_neuron_values)
{
    if(!file.is_open()){
        cout<<"File could not be opened. "<<endl;
        return file;
    }

    int link_count=0;

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
            link_count+=layers[i][j]->outweights.size();

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

            for(int k=0; k<layers[i][j]->outweights.size(); k++)//then 5 values representing connection coordinates and weight value
            {
                file<<layers[i][j]->outweights[k]->postsynaptic.row<<" ";//layer to connect from
                file<<layers[i][j]->outweights[k]->postsynaptic.col<<" ";//neuron to connect from
                file<<i<<" ";//layer to connect to
                file<<j<<" ";//neuron to connect to
                file<<layers[i][j]->outweights[k]->_weight<<" ";//weight(double)
            }
        }
    }
    file.close();
    return file;
}


ifstream& net::Open(ifstream& file)
{

    if(!file.is_open())
    {
        cout<<"Network file could not be opened."<<endl;
        return file;
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
    return file;
}










void net::SetWeights(double val, int _layer, int _neuron, int _link)
{
    layers.at(_layer).at(_neuron)->outweights.at(_link)->SetWeight(val);
}

void net::SetWeights(double val, int _layer, int _neuron)
{
    for(int i=0; i<layers.at(_layer).at(_neuron)->outweights.size(); i++)
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

float net::GetWeights( int _layer, int _neuron, int _link)
{
    
    return (float) layers.at(_layer).at(_neuron)->outweights.at(_link)->GetWeight();
}

vector<float> net::GetWeights( int _layer, int _neuron)
{
    vector<float> _weights;
    for(int i=0; i<layers.at(_layer).at(_neuron)->outweights.size(); i++)
    {
        _weights.push_back(GetWeights( _layer, _neuron, i));
    }
    return _weights;

}

vector<float> net::GetWeights( int _layer)
{
    vector<float> _weights;
    for(int i=0; i<layers.at(_layer).size(); i++) //layer size
    {
        for(int j=0; j< layers.at(_layer).at(i)->outweights.size(); j++) //neurons outweights
        {
           _weights.push_back(GetWeights(_layer, i, j));
        }
    }
    return _weights;
}

vector<vector<float>> net::GetWeights(){
    vector<vector<float>> _weights;
    for(int i = 0; i< layers.size(); i++){
        _weights.push_back(GetWeights(i));
        
    }
    return _weights;
}



void net::CleanAfterBuild()
{
    cout<<"Finished building net. Deleting extra memory used in build process."<<endl;
    
     groups.resize(0);

}

//todo:fix to use new random generator
// void net::RandomizeWeights(int _layer, int _neuron, int _link)
// {
//  layers.at(_layer).at(_neuron)->outweights.at(_link)->SetRandWeight();
// }

// void net::RandomizeWeights(int _layer, int _neuron)
// {
//     for(int i=0; i<layers.at(_layer).at(_neuron)->outweights.size(); i++)
//     {
//         RandomizeWeights(_layer, _neuron, i);
//     }

// }

// void net::RandomizeWeights(int _layer)
// {
//     for(int i=0; i<layers.at(_layer).size(); i++)
//     {
//         RandomizeWeights(_layer, i);
//     }

// }

// void net::RandomizeWeights()
// {
//     for(int i=0; i<layers.size(); i++)
//     {
//        RandomizeWeights(i);
//     }

// }


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
