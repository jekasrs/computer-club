#!/bin/bash

# Цикл по всем файлам в текущей директории с расширением ".txt"
i=1
for filename in *.txt; do
    echo -e "File #$i: $filename\nRESULTS:"
     i=$((i+1))
    ../build/my_system "$filename"
done