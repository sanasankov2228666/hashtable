set terminal pngcairo size 2000,600
set output 'dump/table_hash_func_asci_sum_517/histogram.png'
set style fill solid 0.7
set boxwidth 1.0 relative
set xlabel "Списки"
set ylabel "Количество слов"
set title "Гистограмма распределения слов"
set grid ytics
set xtics rotate by -45
set xrange [0:*]
set yrange [0:*]
plot 'dump/table_hash_func_asci_sum_517/data.txt' using 1:2 with impulses lc rgb "blue" title "Хэш-таблица"
