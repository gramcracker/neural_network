# neural_network
This is my personal project to make a realatively simple neural network framework in C++.
I created it specifically for experimentation with real time natural language processing 
and sementic analysis, although it is still a work in progress.

I created this because I needed a framework that could be configured in complex and irregular ways for easy experimentation. In order to suit my needs, it has have the following features:

* Layer by layer syntax. ✓
* Copy and paste of any portion of the net. ✓
* Ability for network to build or erase connections. x
* Ability to change structure at runtime. x
* Realtime input and output. ✓
* Loading/saving of weight values and structure to file. ✓
* Spike timing based activation. x
* Focus on learning strategies for natural language processing and semantic analysis. ✓

As stated above, this is still a work in progress, and I have yet to implement all of these features.


## Getting Started
### Prerequisites
You will need support for c++ >= 11. This works best with g++ >= 11, and the pthread library.

### Installing
```
cd build
cmake ..
make
```
then copy the generated neuralnetwork.so + header files to your project or library path. E.g.
put neuralnetwork.so in ```usr/local/lib/``` and .h files into ```usr/local/include/```
then run ```ldconfig```, #include <neuralnetwork/net.h> in your program and compile.

### Usage
Examples on how to use this library can be found in neural_network/examples/.
