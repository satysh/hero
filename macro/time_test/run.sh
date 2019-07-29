#!/bin/bash

outdir=output

nev=1
nth=3

cd ../../../build/
make -j${nth}
wait
cd -
echo -e "\e[1m\e[32m========== All compiled! ================ \e[0m"

cd ../../geometry
rm -fv time_test_geo.root
cd -

if [ -d ${outdir} ];then
    rm -fv ${outdir}/*
else
    mkdir ${outdir}
fi

echo -e "\e[1m\e[32m========== Cleanup finished! ============ \e[0m"

root -l -b -q geo/geo.C
wait
echo -e "\e[1m\e[32m========== Geometry created! ============ \e[0m"

echo -e "\e[1m\e[32m========== Simulation started! ========== \e[0m"
root -l -b -q "sim.C(${nev}, \"${outdir}\")" > ${outdir}/out.txt 2> ${outdir}/err.txt &
sleep 1
echo -e "\e[1m\e[31m... \e[0m"
sleep 1
echo -e "\e[1m\e[31m..... \e[0m"
sleep 1
echo -e "\e[1m\e[31m....... \e[0m"
wait
echo -e "\e[1m\e[32m========== Simulation finished! ========== \e[0m"
