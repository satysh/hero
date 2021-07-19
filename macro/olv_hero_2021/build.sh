#!/bin/bash

cd ../../../build
make -j4
wait
. config.sh
cd -

