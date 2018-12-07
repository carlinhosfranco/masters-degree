set terminal postscript eps enhanced solid color font 'Helvetica,20'
set output 'performance.eps'
set style data dots
set encoding utf8
set key outside left bottom horizontal center reverse enhanced autotitles nobox
set grid
set boxwidth 0.3
#set xrange [0:9]
set yrange [0:32]
set y2range [0:3]



set y2tic

set title "Sieve of Eratosthenes"
set y2label "Efficiency" offset 0.1,1,1
set xlabel "Nodes" offset 0.3,0,1
set ylabel "Speedup" offset 1,1,1

set style fill transparent pattern 4 border

set style line 1 lt 1 lc rgb "#DC143C" lw 3             #RED
set style line 3 lt 3 lc rgb "#000000" lw 3            #black
set style line 2 lt 1 lc rgb "#4169E1" lw 3			 #RoyalBlue

#for than on graph
#set multiplot layout 3,3 

plot './efficiency.dat' using 1:2:xtic(1) with boxes ls 1 title "Efficiency" axes x1y2, '' using 1:2:2 with labels center offset 0,0.3 font 'Helvetica,20' notitle axes x1y2\
	,'./speedup.dat' using 1:2 with linespoints ls 2 title "Speedup" axes x1y1\
	,'./speedup.dat' using 1:1 with linespoints ls 3 title "Speedup Ideal" axes x1y1\


