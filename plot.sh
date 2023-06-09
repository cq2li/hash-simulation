#!/bin/sh
# echo Please give a description of the setup
# read description
file=$(date +"%Y%m%d_%H%M%S%p_plot").txt
echo k = 1 > $file
./sim_plot.o 1000000 1 100000000 1996 >> $file
file=$(date +"%Y%m%d_%H%M%S%p_plot").txt
echo k = 4 > $file
./sim_plot.o 1000000 4 100000000 1996 >> $file
file=$(date +"%Y%m%d_%H%M%S%p_plot").txt
echo k = 9 > $file
./sim_plot.o 1000000 9 100000000 1996 >> $file
file=$(date +"%Y%m%d_%H%M%S%p_plot").txt
echo k = 19 > $file
./sim_plot.o 1000000 19 100000000 1996 >> $file
file=$(date +"%Y%m%d_%H%M%S%p_plot").txt
echo k = 24 > $file
./sim_plot.o 1000000 24 100000000 1996 >> $file
file=$(date +"%Y%m%d_%H%M%S%p_plot").txt
echo k = 49 > $file
./sim_plot.o 1000000 49 100000000 1996 >> $file
file=$(date +"%Y%m%d_%H%M%S%p_plot").txt
echo k = 99 > $file
./sim_plot.o 1000000 99 100000000 1996 >> $file
file=$(date +"%Y%m%d_%H%M%S%p_plot").txt
echo k = 199 > $file
./sim_plot.o 1000000 199 100000000 1996 >> $file
