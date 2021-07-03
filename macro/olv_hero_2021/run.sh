#!/bin/bash
nev=101
index=1
OUTDIR=output

cd ../../../build
make -j4
cd -
wait

cd geo/
root -l -q geo.C
wait
cd ../

if [ -d ${OUTDIR} ];then
        rm -fv ${OUTDIR}/*
else
        mkdir ${OUTDIR}
fi
echo -e "\e[1m\e[32m========== Clean finished =========== \e[0m"

root -l -q "sim.C(${nev}, ${index})"
wait

root -l eventDisplay.C
