#!/bin/bash
thrnum=1

rm -fv eventDisplay.root sim.root par.root

cp output_parallel/sim_${thrnum}.root sim.root
cp output_parallel/par_${thrnum}.root par.root

root -l eventDisplay.C

rm -fv eventDisplay.root sim.root par.root
