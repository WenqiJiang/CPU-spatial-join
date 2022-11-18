set terminal postscript eps enhanced color 24

#set title "Varying Num. Grid Cells in SGrid"

set xlabel "Grid cells per side"
set ylabel "Avg. Time per Tick (s)"
set xrange [0:128]
set size 0.6, 1
set output "out/eps/SGrid/NumSGridCellsLarge.eps"

plot "out/outfiles/SGrid/NumSGridCellsLargeAvg.txt" using 1:(($3+$5+$7)/100.0) notitle with linespoints lw 7

