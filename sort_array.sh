#!/bin/bash

echo "Enter the array element separated by spaces :"
read -a arr

n=${#arr[@]}

for ((i = 0; i < n; i++)); do
    for ((j = 0; j < n - i - 1; j++)); do
        if [ ${arr[j]} -lt ${arr[j+1]} ]; then
            # Swap arr[j] and arr[j+1]
            temp=${arr[j]}
            arr[j]=${arr[j+1]}
            arr[j+1]=$temp
        fi
    done
done

echo "Array in descending order: ${arr[@]}"