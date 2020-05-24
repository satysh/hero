#!/bin/bash

# script for run sim with diff geo from 0.6 thick to 6 thick micron

nev=6
NTHR=3
ionIndex=3

cd ../../../build
make -j3
cd -
echo -e "\e[1m\e[32m========== Compilation finished =========== \e[0m"

for i in $(seq 0 1);do
  OUTDIR=output_paralell_${i}
  if [ -d ${OUTDIR} ];then
    rm -fv ${OUTDIR}/*
  else
    mkdir ${OUTDIR}
  fi
  echo -e "\e[1m\e[32m========== Clean finished (${i})=========== \e[0m"


  for thr in $(seq 1 ${NTHR}); do
    echo "Starting simulation in thread ${THR}"
    root -l -b -q "sim.C(${nev}, ${thr}, \"${OUTDIR}\", ${ionIndex}, ${i})" > ${OUTDIR}/out_${thr}.txt 2> ${OUTDIR}/err_${thr}.txt &
  done
  wait
done
wait

echo -e "\e[1m\e[32m========== Simulations are finished =========== \e[0m"
exit
