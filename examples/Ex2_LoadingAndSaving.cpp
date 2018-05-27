#include <iostream>
#include "net.h"
#include "file_manager.h"
using namespace std;
int main(){

	ofstream outputFile("weights");
	ifstream inputFile("weights");
	vector<double>input{ .5, .3, 0, 2};
	vector<double>output{ 0, 1, 0, 0, 1, 0};
	vector <int> dims{ 4, 5, 6};
	net n1(dims, true);
	n1.SetBias(1,1);
	n1.TrainBackpropagation( output, input);
	cout<<"n1:"<<endl;
	for (int i= 0; i<n1.get_size(); i++){
		cout<<"layer "<<i<<" = "<<n1.get_size(i)<<endl;
	}
	n1.Save(outputFile, false);

	net n2;
	n2.Open(inputFile);
	cout<<"n2:"<<endl;
	for (int i= 0; i<n2.get_size(); i++){
		cout<<"layer "<<i<<" = "<<n2.get_size(i)<<endl;
	}



	return 0;
}
