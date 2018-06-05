#include <iostream>
#include "net.h"
using namespace std;
int main(){
	/*this example shows how to create simple network from two or more pre-existing 
	networks in a copy/paste fashion by using the "=" operator , and the methods 
	FullyConnectToNet and AppendNet
	*/

	vector <int> dims{ 3, 5, 4};
	vector <double> in{ 0, 2, 3};
	vector <double> out{ 1, 0, .5, 0};
	net n1(dims, true);

	//output the shape of n1
	cout<<"n1:"<<endl;
	for (int i= 0; i<n1.get_size(); i++){
		cout<<"layer "<<i<<" = "<<n1.get_size(i)<<endl;
	}

	//create n2 in the shape of n1 ( "=" does NOT copy over pointers to neurons)
	net n2 = n1;
	//net n2(n1);//also clones n1

	//output the shape of n2
	cout<<"n2:"<<endl;
	for (int i= 0; i<n2.get_size(); i++){
		cout<<"layer "<<i<<" = "<<n2.get_size(i)<<endl;
	}
	

	int n1LayerFrom = n1.get_size()-1;
	int n2LayerTo = 0;
	bool makeNewNet = true;

	/*connect the two nets allowing for them to pass values among eachother
	e.g. n1.FeedForeward then n2.FeedForeward would pass data in the same way
	that one continuous net would
	*/
	n1.FullyConnectToNet(&n2, n1LayerFrom, n2LayerTo);


	net n3;
	/*create pointers to the layers in n1 and n2 in an vector in n3
	allowing n3 to control them as one net
	*/
	n3.AppendNet(&n1,false);
	n3.AppendNet(&n2,false);

	//the shape of n3 is the same as n1 + n2
	cout<<"n3:"<<endl;
	for (int i= 0; i<n3.get_size(); i++){
		cout<<"layer "<<i<<" = "<<n3.get_size(i)<<endl;
	}

	//train n1 and n2 by calling TrainBackpropagation on n3
	n3.TrainBackpropagation(out,in);
	cout<<endl<<"n1 output = ";
	n1.Output();//original
	cout<<endl<<"n2 output = ";
	n2.Output();//original
	cout<<endl<<"n3 output = ";
	n3.Output();//will be the same output as the last net that was appended (n2)
	
	
	return 0;
}
