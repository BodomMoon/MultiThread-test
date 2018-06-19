set terminal 'png'
set title 'Array size(2^n)'
set datafile separator " "
set output 'time4.png'
set ylabel 'TimeCost'
plot [:][:] "time2.txt" using 1:2 with linespoints linewidth 2 title 'Time cost'
