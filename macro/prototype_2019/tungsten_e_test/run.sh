#!/bin/bash
nev=1
index=1
OUTDIR=output

if [ -d ${OUTDIR} ];then
        rm -fv ${OUTDIR}/*
else
        mkdir ${OUTDIR}
fi
echo -e "\e[1m\e[32m========== Clean finished =========== \e[0m"

cd ../../../../build
make -j3
cd -
echo -e "\e[1m\e[32m========== Compilation finished =========== \e[0m"

#root -l -b -q "sim.C(${nev}, ${index})" 2> err.txt
root -l -b -q "sim.C(${nev}, ${index}, \"${OUTDIR}\")" > ${OUTDIR}/out.txt 2> ${OUTDIR}/err.txt
wait

echo -e "\e[1m\e[32mcat err.txt:\e[0m"
cat ${OUTDIR}/err.txt
echo -e "\e[1m\e[32m========== Simulation finished =========== \e[0m"

