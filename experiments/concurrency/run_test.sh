#!/usr/bin/env bash
iterations=10
echo "Running test:"

for i in $( eval echo {0..$iterations} )
do
    ./a.out
done
echo "Iterations done."
