#!/bin/bash
nev=100000
index=1
OUTDIR=output

cd ../../../build
make -j4
cd -
wait

#cd geo/
#root -l -q geo.C
#wait
#echo -e "\e[1m\e[32m========== geo build finished =========== \e[0m"
#cd ../

if [ -d ${OUTDIR} ];then
        rm -fv ${OUTDIR}/*
else
        mkdir ${OUTDIR}
fi
echo -e "\e[1m\e[32m========== Clean finished =========== \e[0m"

root -l -q "sim.C(${nev}, ${index})" > ${OUTDIR}/out.txt 2> ${OUTDIR}/err.txt
wait

cat ${OUTDIR}/err.txt
echo -e "\e[1m\e[32m========== sim finished =========== \e[0m"
#root -l eventDisplay.C
