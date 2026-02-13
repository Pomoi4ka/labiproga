set terminal svg font "CMU Serif,18"

# define the background grid
set grid lc rgb '#202020' lt 0 lw 1
set border back 3 lc rgb '#000000' lt 1
set tics nomirror in
set tics font ",18"
set grid ytics xtics

# define labels, tics and legend
set pointintervalbox 3
set key above
set xlabel "{/:Italic n}"
set ylabel "{/:Italic a_n}"

set logscale y 1.41

set output 'plot_a_n.svg'
# the plot command
plot \
"table.csv" u "n":"a_n" w lp lw 2 title "{/:Italic a_n}"

set output 'plot_S.svg'
set ylabel "{/:Italic S}"
set logscale y 1.01
plot \
"table.csv" u "n":"S" w lp lw 2 title "{/:Italic S}"
