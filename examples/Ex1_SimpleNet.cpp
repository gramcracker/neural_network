#include <iostream>
#include "net.h"
using namespace std;
int main(){
	/*this example shows how to create simple network in two different ways
	the first way is by using preset dimensions, and the second is layer by layer

	*/
	//using preset dimensions
	vector <int> dims{ 3, 5, 4};//specifies the shape of the net. 3 layers: 3 input 5 hidden and 4 output
	vector <double> in{ 0, 2, 3};
	vector <double> out{ 1, 0, .5, 0};
	net n1(dims, true);//creates fully connected net with weights initialized to random values in the shape of dims
	n1.TrainBackpropagation( out, in);


	//using layer by layer syntax:
	net n2;
	vector <double> in2{ 0, 2, 3, 1, 4, .6, .5, .4, 0};//new expected output has 9 elements
	n2.AddNeurons( 3, 0, "layer1");//layer1 is at the position [0] with 3 neurons
	n2.AddNeurons( 5, 1, "layer2");//create 5 neurons in layer at the position [1]
	n2.FullyConnect( 0, 1);//fully connect the layers at [0] and [1]
	n2.AddNeurons( 4, 2, "layer3"); //create 4 neurons at layer position [2]
	n2.FullyConnect( 0, 3);//fully connect layer at [0] to [3] to make the [1] and [2] layers connect to the first
	n2.TrainBackpropagation( out, in2);
	
	return 0;
}