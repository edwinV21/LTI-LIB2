# Automatically created by pareto

set size ratio 1
set terminal x11 reset
set xlabel "Fitness 1"
set ylabel "Fitness 2"

plot \
  "pareto.pf.data" title "pareto.pf" with steps
