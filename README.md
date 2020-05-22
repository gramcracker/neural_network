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
You will need support for c++ >= 11. This works best with g++ >= 11. You may need to tell your compiler to use the thread library with the -lpthread option.

```
g++ -std=c++11 foo.cpp -lpthread -o foo
```
### Installing
Download the files and place in respective folders then edit the makefile to fit your project.

### Usage
Examples on how to use this library can be found in neural_network/examples/.

Compiling examples (linux):

```
$ cd path/to/neural_network
$ mv examples/ExampleFile.cpp src/main.cpp
$ make
$ ./bin/a.out
```
