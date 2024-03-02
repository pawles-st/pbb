#!/bin/bash

for n in {1000..100000..10000}
do
	./bins $n 1>./results${n}.txt &
done
wait
