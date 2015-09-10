#include <iostream>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <thread>
#include <set>
#include <time.h>
#include <algorithm>

using namespace std;

//prototyping classes because class neuron has a link vector member
class neuron;
class link;



class neuron
{
  public:
  double _desired=0;// desired output
  double learning_rate=7.5; //learning rate

  vector <link*> inweights; //vector of links coming in. you must push back the links to wire the net

  void feed();// feed forward for this neuron
  void reconstruct();
  virtual void throwup();//back propagates for this neuron
  void get_entropy(); //calculates the amount of information received by the neuron
  
  
  virtual void activate() //the sigmoid activation function
  {
		_buffer=(1/(1+(exp(-(_buffer)))));
		information.insert(_buffer);
	

  }
  
  double getval()
  {
	  return(_value);
  }
  

  void set_bias(double bias)
  {
	  _bias=bias;
  }
  
  void setin(double input){_input=input;}// function for setting input
  friend class net;
  friend class lstm_block;// a father must have a close relationship with his offspring
  virtual ~neuron(){};//virtual so destructor for dynamic neuron gets called

  protected:
  multiset<float> information;//multisets can give the number of individual values 
  double _buffer=0;// buffer for value to ensure coreect timeing of activation
  double _ebuffer=0;// buffer for error propagation
  double _gradient=0;
  double _value=0; //the input from array of links or weights
  double _input=0;//input into the network
  double _error=0;//the difference between desired output and the real output
  int entropy=0;// used to give the number of neurons added above this neuron
  double _bias=0;
  double _old_value=0;
  
};

class dynamic_neuron: public neuron//(not copy constructable) this neuron is able to clean up after itself
{
	public:
	//this class is only to be used for automatically constructed nets
	~dynamic_neuron();
};



class link// weights between neurons
{
  public:
  
  link(neuron& in): _origin(in)//cant have a weight floating around without input from a neuron
  {
	setrweight();// all weights are initiated with random vales
  }
  
  void setrweight()
  {
  _weight=(double)rand()/(RAND_MAX);

  }
  
  void setweight(int x){_weight=x;}//for setting the weight value of a link
  friend class net;
  friend class neuron;
  friend class gate;
  friend class lstm_block;
  private:
  double _weight=0;//so that were not reading random values
  double _old_weight;
  double _positive_phase=0;
  double _negative_phase=0;
  neuron& _origin;// the neuron this link is getting values from
  

};


void neuron::feed()// feed forward a single neuron
    {
		if(inweights.size()==0)
		{_buffer=_input+_bias;}
		else{
			for(unsigned int i=0;i<inweights.size();i++)
			{
				_buffer+=((inweights[i]->_weight)*(inweights[i]->_origin._value))+_bias;
			}
			
			_buffer+=_input;
			activate();
			
			
			
			}
        
    }
	
	
void neuron::reconstruct()// feed forward a single neuron
{

	for(unsigned int i=0;i<inweights.size();i++)
	{
		inweights[i]->_origin._buffer+=_value*inweights[i]->_weight;

	}


}
        
class lstm_block: public neuron
{
	
	void feed ()
	{
		
		
		for (int i=0; i <4; i++)
		{
			layer1 [i]*=((inweights[i]->_weight)*(inweights[i]->_origin._value))+_bias;
			
	layer1[i]=(1/(1+(exp(-(layer1 [i])))));
			
		}
		
		
		
	}
	
	void activate()
	{
		n2=n0+n1;
		n1*=n2;
		n3*=n2;
		_value=n3;
	};
	
	//void throwup(){}
	private:
	double n0=1,n1=1,n2=0,n3=1;
	double layer1[4]={n0,n0,n1,n3};
	
	
};




class net//represents a the vector of neurons
{
	
  public:
  
	
  vector<vector<neuron*>> layers;
  vector<neuron*>::iterator col;
  vector<vector<neuron*>>::iterator row;
  vector<neuron*>::reverse_iterator rcol;
  vector<vector<neuron*>>::reverse_iterator rrow;
  vector<thread> threads;
  void set_desired(double desired[]);
  void set_input(double input[]);
  void evopatch();// controls the network structure
  void neuralprune(); //deletes unused neurons, and respectful links
  void backprop();
  void feed_forward();
  void feed_forward( int starting_layer, int ending_layer);
  void train(double input[], double desired[]);//set input, feed all neurons, set desired, backprop
  void contrast_diverge( int starting_layer);
  void display_output();
  double output();
  void run_rbm(int starting_layer);
  private:
   
};






class fully_connected_net :public net
{
	public:
	
	fully_connected_net( vector<int> &dimensions)
	{
		layers.resize(dimensions.size());
		cout<<endl<<"size = "<<dimensions.size();
		for (int i=0; i <dimensions.size(); i++)
		{
			for (int j=0; j <dimensions[i]; j++)
			{
				layers.at(i).push_back(new dynamic_neuron);
				
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
	
	
	
	~fully_connected_net()
	{
			
		for(row=layers.begin(); row!=layers.end(); row++)
		{

			for(col=row->begin(); col !=row->end(); col++)
			{
				
				delete(*col); //im hoping this deletes the node not the iterator
				cout <<endl<<"deleted neuron";
			}
			row->clear();//will this clear the row it points to ?
		}
	layers.clear();
	
	}
	
	
};


dynamic_neuron::~dynamic_neuron()
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






void neuron::throwup()
{
	_error=(_value-_desired)+_error;
	_gradient=((learning_rate)*(_error)*((_value)*(1-(_value))));//calculate error for neuron
	if(inweights.size()>0)
	{
		for(unsigned int i=0;i<inweights.size();i++)
		{
			inweights[i]->_origin._ebuffer+=(inweights[i]->_weight)*(_gradient);//pass the error before weight change
			inweights[i]->_old_weight=inweights[i]->_weight;
			inweights[i]->_weight-=_gradient*(inweights[i]->_origin._value);
			inweights[i]->_origin._desired=(inweights[i]->_origin._value);
			
			
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

		if (threads.size()>0){cout<<endl<<threads.size()<<"threads remain";}
		
		for(col=row->begin(); col !=row->end(); col++)
		{
			((*col)->_value)=((*col)->_buffer);//to avoid accessing and activating a neuron at the same time ( like if a neuron is connected to one in the same layer)
		}
		
	}
	
}

void net::feed_forward( int starting_layer, int ending_layer)
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




void net::backprop()
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


void net::set_input(double input[])
{
	for( int i=0; i<layers.at(0).size(); i++)
	{
        layers.at(0).at(i)->_input=(input[i]);
		
	};
}

void net::set_desired(double desired[])
{
	for( int x=0; x<layers.at(layers.size()-1).size(); x++)
	{
        layers.at(layers.size ()-1).at(x)->_desired=(desired[x]);
	};
}

void net::train(double input[], double desired[])
{
	set_input(input);
	
	feed_forward();

	set_desired(desired);

	backprop();

}

void net::contrast_diverge( int starting_layer)
{

	
	feed_forward(starting_layer, starting_layer+1);
	
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
	
	feed_forward(starting_layer, starting_layer+1);
	
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
	
	
void net::display_output()
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



	

void net::run_rbm(int starting_layer)
{
		feed_forward(starting_layer, starting_layer+1);
		
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



int main ()
{
srand(time(NULL));
  //sample network and training
double desired[1]={.25};
double input[2]={.5,.75};
vector <int> arr={2,3,1};
fully_connected_net net1(arr);
start:
net1.train(input,desired);
net1.display_output();
cin.get();
goto start;



return 0;
  
}
