set terminal png size 800,600
set output 'example_nozzle_plot.png'
set title 'Bell Nozzle Geometry'
set xlabel 'Axial Position (m)'
set ylabel 'Radius (m)'
set grid
set key top left
plot 'nozzle_upper.dat' with lines title 'Upper Contour', \
     'nozzle_lower.dat' with lines title 'Lower Contour'
