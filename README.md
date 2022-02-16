# neural_network
This is a realatively simple neural network framework in C++. 

I created this because I needed a framework that could be configured in complex and irregular ways for easy experimentation. It has have the following features:

* -NEW- now outputs a python wrapper module.
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
-cmake
-You will need support for c++ >= 11. This works best with g++ >= 11, and the pthread library.
-To generate the python wrapper you will also need python3-pybind11 or copy the git repo into the local python folder. If using pybind11 locally, make sure to follow directions in the CMakeLists.txt file.


### building
```
cd build
cmake ..
make
```
### Installing
```
chmod +x install.sh
sudo sh install.sh
```
this just copies the generated neuralnetwork.so file to your project to /usr/local/lib/ and the header files to /usr/local/include/neuralnetwork/

### Usage
Examples on how to use this library can be found in neural_network/examples/.
