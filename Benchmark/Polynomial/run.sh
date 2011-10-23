#!/bin/bash
function plot_data()
{
    name=$1
    gnuplot<<EOF
set term post eps color solid enh
set output '$name.eps'
set title "$name"
set xlabel "Mathematica"
set ylabel "nV"
#plot 2*x title "Required",'$name.result' using 2:1 title ''
plot '$name.result' using 2:1 title ''
EOF
}
function benchmark()
{
    name=$1
    ../../bin/nv $name.u > tmp1
    MathKernel -script $name.m >tmp2
#diff --ignore-all-space tmp1 tmp2
    paste tmp1 tmp2 > $name.result
}
function do_all()
{
    for file in *.u
    do
	#name=${file%.*}
	#benchmark $name
	plot_data $name
    done

}
