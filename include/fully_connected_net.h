#ifndef FULLY_CONNECTED_NET_H
#define FULLY_CONNECTED_NET_H
#include "net.h"
using namespace std;
class fully_connected_net: public net
{
    public:
        fully_connected_net( vector<int> &dimensions);
        void AddNeurons(int amount, int layer);
        ~fully_connected_net();
};

#endif // FULLY_CONNECTED_NET_H
