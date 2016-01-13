#include <iostream>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include "data_operations.h"
#include "net.h"
#include "vocabulary.h"


using namespace std;


int main (int argc, char **argv)
{
    srand(time(NULL));
    //sample network
    vector<int> dimensions={5,6,8};
    net net1(dimensions);



    return 0;

}
