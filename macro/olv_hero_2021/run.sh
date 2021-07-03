#!/bin/bash

cd ../../../build
make -j4
cd -
wait

root -l -q sim.C
wait

root -l eventDisplay.C
