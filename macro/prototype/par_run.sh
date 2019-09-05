#!/bin/bash
nev=10
OUTDIR=output_paralell
NTHR=7

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

for thr in $(seq 1 ${NTHR}); do
	echo "Starting simulation in thread ${THR}"
	root -l -b -q "sim.C(${nev}, ${thr}, \"${OUTDIR}\")" > ${OUTDIR}/out_${thr}.txt 2> ${OUTDIR}/err_${thr}.txt &
done
wait

echo -e "\e[1m\e[32m========== Simulation finished =========== \e[0m"

#root -l histo_paralell.C
