#!/bin/sh
echo Please give a description of the setup
read description
file=$(date +"%Y%m%d_%I%M%S%p").txt
echo $description > $file
./sim.o 1000000 800000 80000000 1996 >> $file &
