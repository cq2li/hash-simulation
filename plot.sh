#!/bin/sh
# echo Please give a description of the setup
# read description
# for k in 1 2 3 4 5
#   do
#     file=${k}_$(date +"%s_plot").txt
#     echo k = $k > $file
#     ./sim_plot.o 1000000 $k $((2 ** 22)) 1996 >> $file
#   done
#
# for k in 9 10 11 19 20 21 24 49 99 ## runs forever, cut down on iterations
#   do
#     file=${k}_$(date +"%s_plot").txt
#     echo k = $k > $file
#     ./sim_plot.o 1000000 $k $((2 ** 21)) 1996 >> $file
#   done

for k in 1 2 3 4 5
  do
    file=${k}_$(date +"%s_plot").txt
    echo k = $k > $file
    ./sim_plot.o 1000000 $k $((2 ** 22)) 1996 >> $file
  done
