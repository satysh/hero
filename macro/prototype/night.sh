#!/bin/bash
nev=3000
OUTDIR=output_paralell
NTHR=3

cd ../../../build
make -j${NTHR}
cd -
echo -e "\e[1m\e[32m========== Compilation finished =========== \e[0m"


if [ -d archive ];then
  if [ -d archive/diffIonsSim ];then
    echo "done"
  else
    mkdir archive/diffIonsSim/
  fi
else
  mkdir archive/
  mkdir archive/diffIonsSim/
fi

for i in $(seq 1 4); do
  rm -fv *.root
  rm -fv *txt
  if [ -d ${OUTDIR} ];then
    rm -fv ${OUTDIR}/*
  else
    mkdir ${OUTDIR}
  fi
  echo -e "\e[1m\e[32m========== Clean finished =========== \e[0m"
  for thr in $(seq 1 ${NTHR}); do
    echo "Starting simulation in thread ${THR}"
    root -l -b -q "sim.C(${nev}, ${thr}, \"${OUTDIR}\", ${i})" > ${OUTDIR}/out_${thr}.txt 2> ${OUTDIR}/err_${thr}.txt &
  done
  wait
  mkdir archive/diffIonsSim/run_${i}/
  cp ${OUTDIR}/* archive/diffIonsSim/run_${i}/
  echo -e "\e[1m\e[32m========== Simulation finished ( index = ${i} ) ============ \e[0m"
  wait
done

