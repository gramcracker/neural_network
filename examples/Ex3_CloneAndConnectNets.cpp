#include <iostream>
#include "net.h"
using namespace std;
int main(){
	/*this example shows how to copy and paste portions of a network by using the + operator/constructor
	operand, and the ConnectToNet method

	*/

	//create the first net
	vector <int> dims{ 3, 5, 4};
	vector <double> in{ 0, 2, 3};
	vector <double> out{ 1, 0, .5, 0};
	net n1(dims, true);

	cout<<"n1:"<<endl;
	for (int i= 0; i<n1.get_size(); i++){
		cout<<"layer "<<i<<" = "<<n1.get_size(i)<<endl;
	}

	//create a new net by cloning
	net n2 = n1;
	cout<<"n2:"<<endl;
	for (int i= 0; i<n2.get_size(); i++){
		cout<<"layer "<<i<<" = "<<n2.get_size(i)<<endl;
	}
	
	//net n2(n1);//also clones n1

	//connect the nets n1 to n2
	/*//(not implemented yet)
	n1.ConnectToNet(n2);//connects n1 output to n2 input
	n1.ConnectToNet( netFrom, layerFrom, Layerto);//also able to connect to specific layers
	*/

	
	return 0;
}