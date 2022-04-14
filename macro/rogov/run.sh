#!/bin/bash
nev=10000
index=1
OUTDIR=output

rm -fv *.root
rm -fv *txt
if [ -d ${OUTDIR} ];then
        rm -fv ${OUTDIR}/*
else
        mkdir ${OUTDIR}
fi
echo -e "\e[1m\e[32m========== Clean finished =========== \e[0m"

cd ../../../build
make -j3
cd -
echo -e "\e[1m\e[32m========== Compilation finished =========== \e[0m"

root -l -b -q "sim.C(${nev}, ${index})" > ${OUTDIR}/out.txt 2> ${OUTDIR}/err.txt
wait

echo -e "\e[1m\e[32m========== Simulation finished =========== \e[0m"
