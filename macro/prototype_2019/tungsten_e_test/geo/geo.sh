#!/bin/bash

rm -fv ../../../../geometry/geo_e_t.root
root -l -q create_tung_geo.C
