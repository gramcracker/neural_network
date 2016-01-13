#ifndef LINK_H
#define LINK_H

#include "neuron.h"
using namespace std;

class neuron;


class link// weights between neurons
{
  public:
  link(neuron& in): _origin(in)
  {
      setrweight();
  }//cant have a weight floating around without input from a neuron

  neuron& _origin;

  void setrweight();
  void setweight(int x);

  friend class net;
  friend class neuron;
  friend class gate;
  friend class lstm_block;

  private:
  double _weight=0;//so that were not reading random values
  double _old_weight;
  double _positive_phase=0;
  double _negative_phase=0;
  // the neuron this link is getting values from


};

#endif
