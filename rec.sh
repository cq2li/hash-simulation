#!/bin/sh
echo Please give a description of the setup
read description
file=$(date +"%Y%m%d_%H%M%S%p_rec").txt
echo $description > $file
./sim.o 1000000 4 100000000 1996 >> $file &
