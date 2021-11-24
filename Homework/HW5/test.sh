#!bin/bash

gcc -Wall -g hw5.c -lm
for i in 2 3 4 5 6 7 8 9 10 11 12 13 14
do
    echo "\nTest $i:"
    ./a.out inputs/test$i.txt>output$i.txt
done