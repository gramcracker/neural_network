#include "data_operations.h"
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <random>
using namespace std;

//todo: work on this
double data_operations::random_number_generator::generate(){
    mt19937 gen((random_device())());
    uniform_real_distribution<> dis(-1.0, 1.0);
    return dis(gen);
    }

//todo:make these use random number generator
double data_operations::random_data()
{
    return (double)rand()/(double)RAND_MAX;
}

void data_operations::make_random(vector<vector<double>>& data)
{
    for(vector<vector<double>>::iterator row=data.begin(); row!=data.end(); row++)
    {
        for(vector<double>::iterator col=row->begin(); col!=row->end(); col++)
        {
            (*col)=random_data();
        }
    }
}

void data_operations::make_random(vector<double>& data)
{
    for(int i=0; i<data.size(); i++)
    {
        data[i]=random_data();
    }
}




void data_operations::make_binary(double limit, double data)
    {
        data>=limit?data=1:data=0;
    }

void data_operations::hopfield_data(double data)
    {
        data<=0?data=(-1):data=1;
    }

void data_operations::maxpool(vector<vector<double>>& data, vector<vector<double>>& target, int windowx=1, int windowy=1)
    {

        int kmax=windowx*windowy;
        double max=0;
        int k=0;
        int l=0;
        int m=0;
		int windownumx=1;
		int windownumy=1;
        for(int i=0; i<data.size(); i++)
        {

            for(int j=0; j<data.at(i).size(); j++)
            {


                if(l==target.size())break;


				if(data[i][j]>max)
				{
					max=data[i][j];
				}
				target[l][m]=max;


				if (j==(windowx*windownumx)-1)
                {


					if(j==data[0].size()-1&&i==(windownumy*windowy)-1)
					{

						j=-1;
						windownumx=1;
						windownumy+=1;
						k=-1;
						max=0;
						m+=1;
						i=(windownumy*windowy)-windowy;


					}
					else
					{

						i+=1;

						j=(windownumx*windowx)-windowx-1;


					}

                }

				if (k==kmax-1)
                {

                    k=-1;
                    windownumx+=1;

                     m+=1;
                    j=(windownumx*windowx)-windowx-1;
					i=(windownumy*windowy)-windowy;





                    max=0;


                }


				if(m==target[0].size())
				{
					m=0;
					l+=1;
				}



				k+=1;



                if(i>=data.size() && j>=data[0].size()) break;


            }
        }

    }

