set terminal 'png'
set title 'Time cost(second)'
set datafile separator " "
set output 'time2.png'
set ylabel 'thread_number'
plot [:][:] "muti_opt.txt" using 1:2 with linespoints linewidth 2 title 'Time cost'
