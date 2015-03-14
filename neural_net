/*todo:
create functions to control creation and destruction of links and neurons by calculating the amount of information going to an input neuron by using the information entropy equation. the function should run at a regular interval for a specified time. at the end of the interval it takes the average entropy. the average would then be equal to the amount of neurons to add. 
*/
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <vector>
#include <fstream>
#include <cmath>
#include <thread>
using namespace std;

class neuron;
class link;



class neuron
{
  public:
  double _desired=0;
  double learning_rate=1;

  vector <link*> inweights;

  void feed();
  void throwup();
  
  virtual void activate_sigmoid()
  {
		_activated=(1/(1+(exp(-(_value)))));

  }
  
  virtual void activate_arctan()
  {
		_activated=(tanh(_value));
		cout<<_activated;
  }


  void setin(double input){_value=input;}
  friend class net;
  
  private:
  
  double _value=0;
  double _activated=0;
  double _deltaneuron=0;
  double _error=0;
};


class link
{
  public:
  link(neuron& in): _origin(in)
  {
	setrweight();
  }
  
  void setrweight()
  {
  _weight=(double)rand()/(RAND_MAX);
  cout<<endl<<this<<"weight="<<_weight;
  }
  
  void setweight(int x){_weight=x;}
  friend class net;
  friend class neuron;
  private:
  double _weight=0;
  double _deltaweight=0;
  double error=0;
  neuron& _origin;
  

};


void neuron::feed()
    {
        for(unsigned int i=0;i<inweights.size();i++)
        {
             neuron::_value+=((neuron::inweights[i]->_weight)*(neuron::inweights[i]->_origin._value));
        }
        
		neuron::activate_sigmoid();
		cout<<endl<<"thread"<<this<<" joined, activated="<<_activated;
        
    }





class net
{

  public:
  vector<vector<neuron*>> layers;
  vector<neuron*>::iterator col;
  vector<vector<neuron*>>::iterator row;
  vector<neuron*>::reverse_iterator rcol;
  vector<vector<neuron*>>::reverse_iterator rrow;
  vector<thread> threads;
  void evopatch();// controls the network stucture
  void neuralprune(); //deletes unused neurons, and respectful links
  void backprop();
  void feed_forward();
  void automate(double input[], double desired[]);//set input, feed all neurons, set desired, backprop
  private:
   
};




void neuron::throwup()
{
	_error=_desired-_value;//calculate error for neuron
	if(inweights.size()>0)
	{
		for(unsigned int i=0;i<inweights.size();i++)
		{
		inweights[i]->_weight+=((neuron::learning_rate) *(neuron::_error) * (neuron::inweights[i]->_origin._value) * 	(neuron::inweights[i]->_weight));
		inweights[i]->_origin._value=0;
		inweights[i]->_origin._desired+=_error*(inweights[i]->_weight);
		cout<<endl<<"origin desired="<<inweights[i]->_origin._desired;
		}
	}	
}



void net::feed_forward()
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
		if (threads.size()==0){cout<<endl<<"thread erased";}
		else{cout<<endl<<threads.size()<<"remains";}
	}
	
}




void net::backprop()
{ 
cout<<endl<<"bprop begin";
	
	for(rrow=layers.rbegin(); rrow!=layers.rend(); rrow++)
	{
		for(rcol=rrow->rbegin(); rcol !=rrow->rend(); rcol++)
		{
			threads.push_back(thread(&neuron::throwup,(*rcol)));
		}
		
		for(int i=0; i<threads.size(); i++)
		{
			threads[i].join();
			cout<<endl<<"thread"<<i<<"joined";
		}
		threads.erase(threads.begin(),threads.end());//destroys threads in array to leave it epmty for next layer
		
		if (threads.size()==0){cout<<endl<<"thread erased";}
		else{cout<<endl<<threads.size()<<"remains";}
	}
}



void net::automate(double input[], double desired[])
{
	for( int i=0; i<layers.at(0).size(); i++)
	{
        layers.at(0).at(i)->_value=(input[i]);
		cout<<endl<<"neuron"<<i<<"value="<<layers.at(0).at(i)->_value;
	};
	
	feed_forward();
	cout<<endl<<"feeding complete";
	
	for( int x=0; x<layers.at(layers.size()-1).size(); x++)
	{
        layers.at(layers.size ()-1).at(x)->_desired=(desired[x]);
	};
	
	cout<<endl<<"desired out set";
	backprop();
	cout<<"done";
}





int main ()
{
srand(time(NULL));
  //sample network and training
neuron n1;
neuron n2;
neuron n3;
neuron n4;

link l1(n1);
link l2(n2);
link l3(n3);
n4.inweights.push_back(&l1);
n4.inweights.push_back(&l2);
n4.inweights.push_back(&l3);
vector<neuron*> layer1;
vector<neuron*> layer2;
layer1.push_back(&n1);
layer1.push_back(&n2);
layer1.push_back(&n3);
layer2.push_back(&n4);
net net1;
net1.layers.push_back(layer1);
net1.layers.push_back(layer2);
double input[3]= {0,0,2};
double desired[1]={1};
net1.automate(input, desired);
cout<<"sample training finished";


  
}
