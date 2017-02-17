set terminal png
set output "diff.png"
set key right center
plot cos(x),diff(cos(x),x)