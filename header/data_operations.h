#ifndef DATA_OPERATIONS_H
#define DATA_OPERATIONS_H
#include <vector>
#include <stdlib.h>
#include <iostream>

using namespace std;

class data_operations{
public:
	class random_number_generator{
	public:
		double min = -1.0;
	    double max = 1.0;
	    double generate();
	};

	void make_binary(double limit, double data);


	void make_random(vector<vector<double>>& data);

	void make_random(vector<double>& data);

	void hopfield_data(double data);

	void maxpool(vector<vector<double>>& data,vector<vector<double>>& target, int windowx, int windowy);

	void maxpool(vector<double> data, int windowx);

	double random_data();
};


#endif // DATA_OPERATIONS_H
