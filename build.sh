#!/bin/sh

echo "compilling program"
cmake . && make &&\
echo "running program" &&\
./AStar;

