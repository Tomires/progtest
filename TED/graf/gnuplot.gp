set multiplot
set timefmt "%H:%M:%S"
set xdata time
set origin 0,0
set size 0.5,1
set title "Overview"
unset ytics

set xlabel "Timestamp of measurement"
set style data lines
set xtics format "%H:%M:%S"
set xtics rotate by -270
set key bottom center
set key outside
set key title "Legend"
set key box width 1 height 1

plot "data.tit" using 1:($2+273.15) axes x1y1 w lines title "temperature",\
"" using 1:4 axes x1y1 w lines title "load average per minute",\
"" using 1:3 axes x1y2 w lines title "CPU fan speed"

set origin 0.5,0
set title "Load vs. temperature"
set ylabel "temperature [K]"
set y2label "load"
set ytics nomirror
set y2tics

plot "data.tit" using 1:($2+273.15) axes x1y1 w linespoints title "temperature",\
"" using 1:4 axes x1y2 w linespoints title "load average per minute"
unset multiplot
