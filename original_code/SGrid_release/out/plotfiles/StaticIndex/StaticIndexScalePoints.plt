set terminal postscript eps enhanced color 24

set output "out/eps/StaticIndex/StaticIndexScalePoints.eps"
#set title "Scaling the Number of Points"

set key left top

set xlabel "Num. of Points"
set ylabel "Avg. Time per Tick (s)"

set xtics   ("10K" 10000, "30K" 30000, "50K" 50000, "70K" 70000, "90K" 90000)

#set style line 5 lt rgb "cyan" lw 7 pt 5 # original Simple Grid's performance
#set style line 10 lt rgb "black" lw 7 pt 6 # shows final SGrid's performance
#set style line 11 lt rgb "green" lw 7 pt 12
#set style line 12 lt rgb "blue" lw 7 pt 8
#set style line 13 lt rgb "red" lw 4 pt 6

# drs: uncommented and appended one line for SGrid
plot "out/outfiles/StaticIndex/StaticIndexScalePointsAvg.txt" \
         using 1:(($3+$5+$7)/100.0) with linespoints title "Binary Search" ls 10 lw 7 pt 1 ps 1.4 lc rgb 'red', \
      "" using 1:(($21+$23+$25)/100.0) with linespoints title "R-Tree" ls 11 pt 12 lw 7 lc rgb 'green', \
      "" using 1:(($39+$41+$43)/100.0) with linespoints title "CR-Tree" ls 12 pt 2 lw 7 lc rgb 'blue',\
      "" using 1:(($57+$59+$61)/100.0) with linespoints title "Linearized KD-Trie" ls 3 pt 5 ps 1.4 lw 7 lc rgb 'magenta', \
      "" using 1:(($75+$77+$79)/100.0) with linespoints title "BEFORE" ls 5 pt 5 ps 1.4 lw 7 lc rgb 'cyan', \
      "" using 1:(($93+$95+$97)/100.0) with linespoints title "AFTER" ls 10 pt 6 lw 7 lc rgb 'black'



# drs: this was used initialy uncommented (except the last 3 lines):
#plot "out/outfiles/StaticIndex/StaticIndexScalePointsAvg.txt" using 1:(($3+$5+$7)/100.0):(($9+$11+$13)/100.0):(($15+$17+$19)/100.0) with errorbars ls 1 lw 7 notitle,\
#     "" using 1:(($3+$5+$7)/100.0) with lines title "Binary Search" ls 1 lw 7,\
#     "" using 1:(($21+$23+$25)/100.0):(($27+$29+$31)/100.0):(($33+$35+$37)/100.0) with errorbars ls 2 lw 7 notitle,\
#     "" using 1:(($21+$23+$25)/100.0) with lines title "R-Tree" ls 2 lw 7,\
#     "" using 1:(($39+$41+$43)/100.0):(($45+$47+$49)/100.0):(($51+$53+$55)/100.0) with errorbars ls 3 lw 7 notitle,\
#     "" using 1:(($39+$41+$43)/100.0) with lines title "CR-Tree" ls 3 lw 7,\
#     "" using 1:(($57+$59+$61)/100.0):(($63+$65+$67)/100.0):(($69+$71+$73)/100.0) with errorbars ls 4 lw 7 notitle,\
#     "" using 1:(($57+$59+$61)/100.0) with lines title "Linearized KD-Trie" ls 4 lw 7#, \
#     # "" using 1:(($75+$77+$79)/100.0):(($81+$83+$85)/100.0):(($87+$89+$91)/100.0) with errorbars ls 5 lw 7 notitle,\
#     # "" using 1:(($75+$77+$79)/100.0) with lines title "Simple Grid" ls 5 lw 7
#     
#     #     "" using 1:($21+$23+$25) with linespoints title "OrthogonalRangeTree" lw 7,\
