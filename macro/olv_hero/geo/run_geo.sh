#!/bin/bash

rm -fv ../../../geometry/OLV_detector.geo.root 
#root -l -b -q geo.C
root -l -b -q geo_solid.C
