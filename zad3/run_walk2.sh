#!/bin/bash

./walk2 100 >./results2_100.txt &
./walk2 1000 >./results2_1000.txt &
./walk2 10000 >./results2_10000.txt &
./walk2 100000 >./results2_100000.txt &

wait
