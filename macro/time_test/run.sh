#!/bin/bash

outdir=output

nev=10
nth=1

cd ../../../build/
make -j3
wait
cd -
echo "All compiled!"

cd ../../geometry
rm -fv time_test_geo.root
cd -

if [ -d ${outdir} ];then
    rm -fv ${outdir}/*
else
    mkdir ${outdir}
fi

echo "Cleanup finished!"

root -l -b -q geo/geo.C
wait
echo "Geometry created!"

for i in $(seq 1 ${nth});do
    root -l -b -q "sim.C(${nev}, ${i}, \"${outdir}\")" > ${outdir}/out_${i}.txt 2> ${outdir}/err_${i}.txt &
    echo "Simulation ${i}"
done
wait
echo "Simulations finished!"
