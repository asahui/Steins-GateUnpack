#!/bin/bash

for i in {270..431}
do
    ./png "BG.CPK.out"$i $i".png"
    echo "output: $i.png"
done
