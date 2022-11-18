set terminal postscript eps enhanced color 24

set output "out/eps/StaticIndex/StaticIndexGauss.eps"

#set title "Varying the Data Skew"

set key right top


set xlabel "Number of Hotspots, logscale"
set logscale x


set ylabel "Avg. Time per Tick (s)"


# plot "out/outfiles/StaticIndex/StaticIndexGaussAvg.txt" using 1:(($3+$5+$7)/100.0) with linespoints title "Binary Search" lw 7,\
#      "" using 1:(($9+$11+$13)/100.0) with linespoints title "R-Tree" lw 7,\
#      "" using 1:(($15+$17+$19)/100.0) with linespoints title "CR-Tree" lw 7,\
#      "" using 1:(($21+$23+$25)/100.0) with linespoints title "Linearized KD-Trie" lw 7, \
#      "" using 1:(($27+$29+$31)/100.0) with linespoints title "Simple Grid" lw 7, \
#      "" using 1:(($33+$35+$37)/100.0) with linespoints title "SGrid" lw 7

# Here is with error bars
plot "out/outfiles/StaticIndex/StaticIndexGaussAvg.txt" using 1:(($3+$5+$7)/100.0):(($9+$11+$13)/100.0):(($15+$17+$19)/100.0) with errorbars ls 1 lw 7 notitle,\
     "" using 1:(($3+$5+$7)/100.0) with linespoints title "Binary Search" ls 1 lw 7,\
     "" using 1:(($21+$23+$25)/100.0):(($27+$29+$31)/100.0):(($33+$35+$37)/100.0) with errorbars ls 2 lw 7 notitle,\
     "" using 1:(($21+$23+$25)/100.0) with linespoints title "R-Tree" ls 2 lw 7,\
     "" using 1:(($39+$41+$43)/100.0):(($45+$47+$49)/100.0):(($51+$53+$55)/100.0) with errorbars ls 3 lw 7 notitle,\
     "" using 1:(($39+$41+$43)/100.0) with linespoints title "CR-Tree" ls 3 lw 7,\
     "" using 1:(($57+$59+$61)/100.0):(($63+$65+$67)/100.0):(($69+$71+$73)/100.0) with errorbars ls 4 lw 7 notitle,\
     "" using 1:(($57+$59+$61)/100.0) with linespoints title "Linearized KD-Trie" ls 4 lw 7, \
     "" using 1:(($75+$77+$79)/100.0):(($81+$83+$85)/100.0):(($87+$89+$91)/100.0) with errorbars ls 5 lw 7 notitle,\
     "" using 1:(($75+$77+$79)/100.0) with linespoints title "Simple Grid" ls 5 lw 7,\
     "" using 1:(($93+$95+$97)/100.0):(($99+$101+$103)/100.0):(($105+$107+$109)/100.0) with errorbars ls 7 lw 7 notitle,\
     "" using 1:(($93+$95+$97)/100.0) with linespoints title "SGrid" ls 7 lw 7

