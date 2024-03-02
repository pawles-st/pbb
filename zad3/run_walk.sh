#!/bin/bash

for N in {5..30..5}
do
	./walk $N >./results${N}.txt &
done

./walk 100 >./results100.txt &

wait
