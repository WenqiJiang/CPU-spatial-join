set terminal postscript eps enhanced color 24
#set terminal png truecolor nocrop enhanced large

#set title "Varying Bucket Size SGrid"

set yrange [0.04:0.16]
set ytics 0.04,0.02,0.16

set xrange [0:35]

set xlabel "Entries per Bucket"
set ylabel "Avg. Time per Tick (s)"
set size 0.6, 1

set output "out/eps/SGrid/SGridBucketSizeLarge.eps"
plot "out/outfiles/SGrid/SGridBucketSizeLargeAvg.txt" using 1:(($3+$5+$7)/100.0) with linespoints notitle lw 7
