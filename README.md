# neural_network
This is my personal project to make a realatively simple neural network framework in C++.
I created it specifically for experimentation with natural language processing in mind, but works for just about anything, although it is still a work in progress.

I created this because I needed a framework that could be configured in complex and irregular ways for easy experimentation. It had have the following features:

* layer by layer syntax.
* copy and paste of any portion of the net.
* ability for network to build or erase connections.
* ability to change structure at runtime.
* realtime input and output.
* loading/saving of weight values and structure to file.
* activation of specific neurons at defines times realative to the network.

As stated above, this is still a work in progress, and I have yet to implement all of these features.


## Getting Started
### Prerequisites
You will need c++11 support. This works best with g++11 and up. You may need to tell your compiler to use the thread library with the -lpthread option.

```
g++ -std=c++11 foo.cpp -lpthread -o foo
```
### Installing
Download the files and place in respective folders then edit the makefile to fit your project.

### Usage
Examples on how to use this library can be found in the examples folder.

Compiling examples (linux):

```
$ cd path/to/neural_network
$ mv examples/ExampleFile.cpp src/main.cpp
$ make
$ ./bin/a.out
```