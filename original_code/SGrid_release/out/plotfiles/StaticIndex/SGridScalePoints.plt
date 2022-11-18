set terminal postscript eps enhanced color 24

set output "out/eps/StaticIndex/SGridScalePoints.eps"
#set title "Scaling the Number of Points"

set key left top

set xlabel "Num. of Points"
set ylabel "Avg. Time per Tick (s)"
set xtics   ("10K" 10000, "30K" 30000, "50K" 50000, "70K" 70000, "90K" 90000)

# define line styles:
# a black line style with linewidth 3, and associated point type 6 (a circle with a dot in it):
set style line 5 lt rgb "cyan" lw 7 pt 5 # original Simple Grid's performance
set style line 10 lt rgb "black" lw 7 pt 6 # shows final SGrid's performance
set style line 11 lt rgb "green" lw 7 pt 12
set style line 12 lt rgb "blue" lw 7 pt 8
set style line 13 lt rgb "red" lw 7 pt 4

plot "out/outfiles/StaticIndex/SGridScalePointsAvg.txt" \
        using 1:(($3+$5+$7)/100.0) with linespoints title "Original" ls 5,\
     "" using 1:(($9+$11+$13)/100.0) with linespoints title "+restructured" ls 11,\
     "" using 1:(($15+$17+$19)/100.0) with linespoints title "+querying" ls 12,\
     "" using 1:(($27+$29+$31)/100.0) with linespoints title "+{/Helvetica-Italic bs} tuned" ls 13,\
     "" using 1:(($21+$23+$25)/100.0) with linespoints title "+{/Helvetica-Italic cps} tuned" ls 10

