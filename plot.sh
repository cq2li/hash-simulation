#!/bin/sh
for k in {1..100..1}
  do
    file=plot_data/v3/${k}_$(date +"%s_plot").txt
    echo k = $k > $file
    ./bin/simulate.o 10000 $k 5000000 1996 >> $file
  done

python3 plot.py
