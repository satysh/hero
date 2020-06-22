#!/bin/bash

nev=10
outdirName=output
if [-d ${outdirName}];then
  rm -fv ${outdirName}/*
else
  mkdir ${outdirName}
fi

cd ~/soft/HERO/build
if (make);then
  echo GOOD
else
  exit
fi
wait
cd -

root -l -q "sim.C(${nev}, 0, \"${outdirName}\")" > ${outdirName}/out.txt 2> ${outdirName}/err.txt

