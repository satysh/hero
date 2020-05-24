#!/bin/bash

for i in $(seq 0 1); do
  root -l -b -q "Prototyp_test_foil_wrapped_plastic.C(${i})"
  wait
done
