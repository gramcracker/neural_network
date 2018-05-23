#include <iostream>
#include "net.h"
using namespace std;
int main(){
	vector <int> dims{ 3, 5, 4};//specifies the shape of the net. 3 layers: 3 input 5 hidden and 4 output
	vector <double> in{ 0, 2, 3};
	vector <double> out{ 1, 0, .5, 0};
	net n1(dims, true);//creates fully connected net with weights initialized to random values in the shape of dims
	n1.TrainBackpropagation( out, in);
	return 0;
}