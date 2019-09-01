#!/bin/sh

catdir=/home/allen/res

for catfile in $catdir/*.txt;do
  cat=`basename $catfile .txt`

	gnuplot --persist <<EOF
	set term post eps color enh solid
	set key top left outside horizontal autotitle columnhead
	datafile="$catfile"
	set output "/home/allen/Time/ $cat.eps"
	unset key
	set title "light curve " . "$cat"
	set xlabel "Time"
	set ylabel "Magnitude"
	set datafile separator ","
	set xdata time
	set timefmt "'%Y-%m-%d %H:%M:%S'" 
	set format x "%m-%d\n%H:%M"
	set yrang [18:8]
	set ytics 1.5
	set autoscale x
	set grid
	plot datafile using 4:5:6 with yerror lc rgb "red"
EOF

done
