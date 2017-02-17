set term png
set output "example1.png"

a=2
f(x)=cos(x)**2
g(x)=sin(a*f(x)+1)

set title "A first example"

set grid
set key center top
set xlabel "Abscissa"
set ylabel "Ordinate"

plot g(x) t "sin(2 cos<sup>2</sup>(x)+1)"