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
	cout<<"n1:"<<endl;
	for (int i= 0; i<n1.get_size(); i++){
		cout<<"layer "<<i<<" = "<<n1.get_size(i)<<endl;
	}


	net n2 = n1;
	cout<<"n2:"<<endl;
	for (int i= 0; i<n2.get_size(); i++){
		cout<<"layer "<<i<<" = "<<n2.get_size(i)<<endl;
	}
	
	//net n2(n1);//also clones n1
	int n1LayerFrom = n1.get_size()-1;
	int n2LayerTo = 0;
	bool makeNewNet = true;
	n1.FullyConnectToNet(&n2, n1LayerFrom, n2LayerTo);
	net n3;
	n3.AppendNet(&n1,false);
	n3.AppendNet(&n2,false);

	cout<<"n3:"<<endl;
	for (int i= 0; i<n3.get_size(); i++){
		cout<<"layer "<<i<<" = "<<n3.get_size(i)<<endl;
	}

	n3.TrainBackpropagation(out,in);
	cout<<endl<<"n1 output = ";
	n1.Output();
	cout<<endl<<"n2 output = ";
	n2.Output();
	cout<<endl<<"n3 output = ";
	n3.Output();
	
	
	return 0;
}