set terminal postscript eps enhanced color 24

set output "out/eps/StaticIndex/SGridGauss.eps"

#set title "Varying the Data Skew"

set key right top

set xlabel "Number of Hotspots, logscale"
set logscale x
set ylabel "Avg. Time per Tick (s)"

# define line styles:
# a black line style with linewidth 3, and associated point type 6 (a circle with a dot in it):
set style line 5 lt rgb "cyan" lw 7 pt 5 # original Simple Grid's performance
set style line 10 lt rgb "black" lw 7 pt 6 # shows final SGrid's performance
set style line 11 lt rgb "green" lw 7 pt 12
set style line 12 lt rgb "blue" lw 7 pt 8
set style line 13 lt rgb "red" lw 7 pt 4

plot "out/outfiles/StaticIndex/SGridGaussAvg.txt" \
        using 1:(($3+$5+$7)/100.0) with linespoints title "Original" ls 5,\
     "" using 1:(($9+$11+$13)/100.0) with linespoints title "+restructured" ls 11,\
     "" using 1:(($15+$17+$19)/100.0) with linespoints title "+querying" ls 12,\
		 "" using 1:(($27+$29+$31)/100.0) with linespoints title "+{/Helvetica-Italic bs} tuned" ls 13,\
     "" using 1:(($21+$23+$25)/100.0) with linespoints title "+{/Helvetica-Italic cps} tuned" ls 10

