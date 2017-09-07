#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "link.h"


using namespace std;


void link::SetRandWeight()
  {
  _weight=(double)rand()/(RAND_MAX);

  }

void link::SetRandWeight(double lower, double upper)
{
    double f = (double)rand() / RAND_MAX;
    _weight = lower + f * (upper - lower);
}



void link::SetWeight(int x)
{
    _weight=x;//for setting the weight value of a link

}

