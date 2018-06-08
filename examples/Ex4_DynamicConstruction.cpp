#include <iostream>
#include <string>
#include "net.h"
using namespace std;
/*
 *This example demonstrates how
 *a net can be built dynamically.
*/
void func(net n){
	int x = 0;
	int y = 0;
	string in;
	cout<<"type -1 to exit."<<endl;
	while(x>=0){
		cout<<"layer number: ";
		start:
		cin>> in;
		x= stoi(in);
		if(x<0) break;
		if(x > (n.get_size())){
			cout<<"enter n where 0 < n >= "<<n.get_size()-1<<endl;
			goto start;
		}
		cout<<"add neurons: ";
		cin>>in;
		y= stoi(in);
		if(y<0) break;
		n.AddNeurons( y, x);
		cout<<"net shape:"<<endl;
		for(int i=0; i<n.get_size(); i++){
			cout<<"layer"<<i<<", length: "<<n.get_size(i)<<endl;
		}
		cout<<endl;
	}

	
}
int main(){
	net n1;
	func(n1);
	return 0;
}