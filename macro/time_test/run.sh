#!/bin/bash

outdir=output

nev=10
nth=3

cd ../../../build/
make -j${nth}
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

root -l -b -q "sim.C(${nev}, \"${outdir}\")" > ${outdir}/out.txt 2> ${outdir}/err.txt &
wait
echo "Simulations finished!"
