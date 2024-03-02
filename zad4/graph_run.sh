#!/bin/bash
# uncomment appropriate section to run the experiment for a given graph

#k=10000 # clique
#gt=0

#k=1000 # path (middle)
#gt=1

#k=1000 # path (end)
#gt=2

#k=10000 # tree
#gt=3

#k=50 # lollipop (simulations for 50 repetitions can take about 5-6 hours)
#gt=4
./graph_walk $gt $k 50 600 >./results${gt}_50.txt &
./graph_walk $gt $k 650 1050 >./results${gt}_650.txt &
./graph_walk $gt $k 1100 1400 >./results${gt}_1100.txt &
./graph_walk $gt $k 1450 1750 >./results${gt}_1450.txt &
./graph_walk $gt $k 1800 2000 >./results${gt}_1800.txt &
wait
