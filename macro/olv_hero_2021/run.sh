#!/bin/bash

cd ../../../build
make -j4
cd -

root -l -q sim.C
root -l eventDisplay.C
