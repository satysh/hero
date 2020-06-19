#!/bin/bash

rm -fv *root .*txt
cd ~/soft/HERO/build
if (make);then
  echo GOOD
else
  exit
fi

wait
cd -

root -l -q sim.C > out.txt 2> err.txt

cat err.txt

subl out.txt
