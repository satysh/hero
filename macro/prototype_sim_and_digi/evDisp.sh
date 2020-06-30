#!/bin/bash
thrnum=1

rm -fv eventDisplay.root sim.root par.root

cp output_paralell/sim_${thrnum}.root sim.root
cp output_paralell/par_${thrnum}.root par.root

root -l eventDisplay.C

rm -fv eventDisplay.root sim.root par.root
