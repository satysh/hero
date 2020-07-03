#!/bin/bash
# alpha 1000020040
# neutron 2112
pid=1000020040
nev=10
INPUTDIR=output_parallel
OUTDIR=pid_${pid}_output_digi
NTHR=3

cd ../../../build
if (make -j${NTHR});then
  echo -e "\e[1m\e[32m========== Compilation finished! =========== \e[0m"
  cd -
else
  echo -e "\e[1m\e[32m========== Compilation is stoped with errors! =========== \e[0m"
  cd -
  exit
fi

if [ -d ${INPUTDIR} ];then
  echo "${INPUTDIR} was found!"
else
  echo "${INPUTDIR} wasn't found!"
  exit
fi

cd ${INPUTDIR}
for ii in $(seq 1 ${NTHR}); do
  if [ -f sim_${ii}.root ]; then
    echo "sim_${ii} was found!"
  else
    echo "sim_${ii} wasn't found!"
    exit
  fi
  if [ -f par_${ii}.root ]; then
    echo "par_${ii} was found!"
  else
    echo "par_${ii} wasn't found!"
    exit
  fi
done
cd ../
echo -e "\e[1m\e[32m========== Inputs checking is finished! =========== \e[0m"

if [ -d ${OUTDIR} ]; then
  rm -fv ${OUTDIR}/*
else
  mkdir ${OUTDIR}
fi
echo -e "\e[1m\e[32m========== Clean finished! =========== \e[0m"

for thr in $(seq 1 ${NTHR}); do
  echo "Starting digitization in thread ${THR}"
  root -l -b -q "digi.C(${nev}, ${thr}, \"${INPUTDIR}\", \"${OUTDIR}\", ${pid})" >${OUTDIR}/out_${thr}.txt 2>${OUTDIR}/err_${thr}.txt &
done
wait

echo -e "\e[1m\e[32m========== Digitization finished! =========== \e[0m"


