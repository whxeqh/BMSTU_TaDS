#!/usr/bin/gnuplot -persist

reset
NO_ANIMATION = 1

set terminal svg size 1920, 1080
set output '../plots/linear_time_flag_buble.svg'

set xlabel "Количество элементов, шт"
set ylabel "Время, мкс"
set grid
set title "Линейный график для измерений времени выполнения сортировки стран"

plot "../data/countries/flag_data.txt" using 1:2 with linespoints pt 9 lt rgb "red" title "Страны, сортировка пузырьком с флагом",\
     "../data/keys/flag_data.txt" using 1:2 with linespoints pt 9 lt rgb "blue" title "Ключи, сортировка пузырьком с флагом",\