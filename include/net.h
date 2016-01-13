#ifndef NET_H
#define NET_H

#include "neuron.h"
#include "link.h"
#include <thread>
#include <vector>

using namespace std;

class net//represents a the vector of vectors of neurons
{


  public:
    net();
    net(vector <int> &dimensions);
    net(vector <int> &dimensions, bool fullyconnected);
    ~net();
    net(const net& tobecloned);
    net& operator=(const net& tobecloned);
    friend net* clone(net& tobecloned);//
    friend class vocabulary;

  vector<neuron*>::iterator col;
  vector<vector<neuron*>>::iterator row;
  vector<neuron*>::reverse_iterator rcol;
  vector<vector<neuron*>>::reverse_iterator rrow;
  vector<thread> threads;

  void Save();
  void Open();
  void SetDesired(vector<double>& desired);   //sets the desired Output of the network for Training
  void SetInput(vector <double>& input);   //sets the input of the network
  void evopatch();   // controls the network structure
  void neuralprune();   //deletes unused neurons and respectful links based on how frequently they hold a value
  void Backpropagate();   // backpropagates the error through the network one layer at a time
  void FeedForeward();   //feeds the input through the network one layer at a time
  void FeedForeward( int starting_layer, int ending_layer);   //feeds the network from a starting layer to an ending layer
  void Train(vector <double>& input, vector <double>& desired);   //set input, feed all neurons, set desired, back-prop
  void ContrastDiverge( int starting_layer);   //performs the contrast divergent algorithm between the layer selected and the next layer
  void Output();   //displays the Output values of the neurons in the last layer
  double Output(int row, int column); //returns the output value of the neuron selected
  void TrainRestricedBoltzman(int starting_layer);  //performs the restricted boltzmann machine algorithm between the layer selected and the next layer
  void OutputToVector(vector<vector<double>>& dataout); //outputs the values of the last layer to a specified vector of vectors
  void OutputToVector(vector<double>& dataout); //outputs the values of the last layer to a specified vector
  void AddNeurons(int amount, int layer);   //adds neurons to a layer
  void AddFullyConnectedNuerons(int amount, int layer); //adds neurons to specified layer and fully conects them to the network
  void Connect(int layer_from, int column_from, int layer_to, int column_to); //adds a link from a neuron to a neuron(in the order in will be Trained)
  void SingleConnect();
  void SingleConnect(int layer_from, int layer_to);
  void FullyConnect();  //fully connects all layers in the net(does not clear out previous links)
  void FullyConnect(int layer_from, int layer_to);  //fully connects a specific portion of the network
  void FullyConnectNets(net* net_from, net* net_to, int layer_from, int layer_to);   //fully connects a specific layer of a net to a specific layer in a seperate net
  int get_size();// returns size of the net
  int get_size(int i);  //returns the size of the layer




  protected:
      // if layers are filled manually, destructor
      //will cause destructor to delete objects twice
      vector<vector<neuron*>> layers;


  private:



};



#endif
