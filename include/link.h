#ifndef LINK_H
#define LINK_H

#include "neuron.h"
using namespace std;

class neuron;

/** embodies the weights between neurons
 *
 * \param takes neuron object that the value will be fed from during FeedForeward()
 *
 */
class link
{
  public:
  link(neuron& in): postsynaptic(in)//cant have a weight floating around without an input neuron
  {

  }

  link(neuron& in, double weight): postsynaptic(in),_weight(weight)//overloaded constructor
  {

  }


  neuron& postsynaptic;

  void SetWeight(double x);/**< sets a specified weight to the object */
  double GetWeight();/**gets the weight */

  friend class net;
  friend class neuron;
  friend class lstm_block;

  private:
  double _weight=0;
  double _old_weight;
  double _positive_phase=0;
  double _negative_phase=0;



};

#endif
