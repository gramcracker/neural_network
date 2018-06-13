#ifndef NET_H
#define NET_H

#include "neuron.h"
#include "link.h"
#include <thread>
#include <fstream>
#include <vector>
#include <string>
#include <tuple>
#include <functional>




using namespace std;

class net //represents a the vector of vectors of neurons
{
public:

    struct Operation
    {
    public:
        Operation(int _row,int _col,void(neuron::*func)()):NeuronFunction(func),row(_row), col(_col)
        {
            isnetfunc=false;
        };//constructor for a neuron function

        /*Operation( void(net::*func)()):NetFunction(func)
        {
            isnetfunc=true;
        };//constructor for a net function*/



        //to address what neuron to run the function on
        int row;
        int col;
        bool isnetfunc=false;

        union
        {
            //pointer to function to run
           void(neuron::*NeuronFunction)();
           std::function<void()> NetFunction;
        };

        //SYNTAX: net_object.AddToSequence( iteration , &net::Function);
            template < typename ...T>  Operation( net* obj, void(net::*func)(T...), T... t)
        {
            isnetfunc=true;
            NetFunction= std::bind(func,obj, t...);

        }


    };




    typedef vector<vector<Operation*>> OperationSequence;
    OperationSequence Sequence;







    neuron::LearningType Learning_Type= neuron::backpropagation;
    net();
    net(vector <int> &dimensions);
    net(vector <int> &dimensions, bool fullyconnected);
    ~net();
    net(const net& tobecloned);
    net& operator=(const net& tobecloned);
    friend net* clone(net& tobecloned);//
    net& operator+(const net toConnect);
    friend net* connectToNet();


  vector<neuron*>::iterator col;
  vector<vector<neuron*>>::iterator row;
  vector<neuron*>::reverse_iterator rcol;
  vector<vector<neuron*>>::reverse_iterator rrow;
  vector<double>hormones; //
  vector<double> desired;
  vector<double> input;
  typedef vector <string> group;
  vector<group>groups;
  bool netPointersCopied=false;
  //vector<thread> threads;




  template< typename ...T> void AddToSequence(int iteration, void(net::*func)(T...),  T... t)
  {
    if(Sequence.size()<iteration+1)
    {
        Sequence.resize(iteration+1);
    }

    Sequence.at(iteration).push_back(new Operation(this, func, t... ));
  }

  void AddToSequence(int iteration, int layer, int first, int last, void(neuron::*func)());
 // void AddToSequence(int iteration, int layer, void(neuron::*func)());

  void RunSequence();
  ofstream& Save(ofstream& file, bool save_neuron_values =false);
  ifstream& Open(ifstream& file);
  void SetDesired(vector<double>& desired);   //sets the desired Output of the network for Training
  void SetInput(vector <double>& input);   //sets the input of the network
  void UserInput();
  void SetNeuronTo(int layer, int column, double val);
  void SetBias(double val, int _layer, int _neuron);
  void SetBias(double val, int _layer);
  void SetBias(double val);
  void SetWeights(double val, int _layer, int _neuron, int _link);
  void SetWeights(double val, int _layer, int _neuron);
  void SetWeights(double val, int _layer);
  void SetWeights(double val);
  void RandomizeWeights(int _layer, int _neuron, int _link);
  void RandomizeWeights(int _layer, int _neuron);
  void RandomizeWeights(int _layer);
  void RandomizeWeights();
  //void evopatch();   // controls the network structure
  //void neuralprune();   //deletes unused neurons and respectful links based on how frequently they hold a value
  void FeedForeward(neuron::ActivationType activation_type); //feeds the input through the network one layer at a time
  void FeedForeward(int starting_layer, int ending_layer, neuron::ActivationType activation_type);
  void FeedForeward( int starting_layer, int ending_layer, int first_neuron, int last_neuron, neuron::ActivationType activation_type);
  void Backpropagate();   // backpropagates the error through the network one layer at a time
  void TrainBackpropagation(vector<double>& _desired, vector <double>& _input);
  void TrainBackpropagation(vector<double>& _desired, vector <double>& _input, int _iterations);
  void ContrastDiverge( int starting_layer);   //performs the contrast divergent algorithm between the layer selected and the next layer
  void Output();   //displays the Output values of the neurons in the last layer
  double Output(int row, int column); //returns the output value of the neuron selected
  void TrainRestrictedBoltzman(int starting_layer);  //performs the restricted boltzmann machine algorithm between the layer selected and the next layer
  void OutputToVector(vector<vector<double>>& dataout); //outputs the values of the last layer to a specified vector of vectors
  void OutputToVector(vector<double>& dataout); //outputs the values of the last layer to a specified vector
  void AddNeurons(int amount, int layer);   //adds neurons to a layer
  void AddNeurons(int amount, int layer, string identifying_tag);
  void AddFullyConnectedNeurons(int amount, int layer); //adds neurons to specified layer and fully conects them to the network
  net* AppendNet(net* net_from, bool pasteOver);
  void Connect(int layer_from, int column_from, int layer_to, int column_to); //adds a link from a neuron to a neuron(in the order in will be Trained)
  void Connect(int layer_from, int column_from, int layer_to, int column_to, float weight);
  void ConnectToNet();
  void SingleConnect();
  void SingleConnect(int layer_from, int layer_to);
  void SingleConnect( string identifying_tag_1, string Identifying_tag_2);
  void SingleConnectToRows(int layer_from, int layer_to);
  void SingleConnectToRows(string identifying_tag_1, string identifying_tag_2);
  void SingleConnectToColumns(int layer_from, int layer_to);
  void SingleConnectToColumns(string identifying_tag_1, string identifying_tag_2);
  void SingleConnectFromRows(int layer_from, int layer_to);
  void SingleConnectFromColumns(int layer_from, int layer_to);
  void FullyConnect();  //fully connects all layers in the net(does not clear out previous links)
  void FullyConnect(int layer_from, int layer_to);  //fully connects a specific portion of the network
  void FullyConnect(int layer_from, int lfstart, int lfend, int layer_to, int ltstart, int ltend);
  void FullyConnect( net* net_to, int layer_from, int layer_to);   //fully connects a specific layer of a net to a specific layer in a seperate net
  void CleanAfterBuild();//cleans up excess memory used for building network
  int get_size();// returns size of the net
  int get_size(int i);  //returns the size of the layer





  protected:
      // if layers are filled manually, destructor
      //will cause destructor to delete objects twice
      vector<vector<neuron*>> layers;


  private:



};



#endif
