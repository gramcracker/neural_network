#!/bin/bash

lib=lib/libneuralnetwork.so
include=include

if test -f "$lib"

then
    cp "$lib" /usr/local/lib/
    cp -a "$include"/. /usr/local/include/neuralnetwork/
else
    echo "library not generated, must run cmake then make first."
fi
