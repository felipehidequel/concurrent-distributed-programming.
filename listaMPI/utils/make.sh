#!/bin/bash

if [ $# -lt 2 ]; then
    echo "Usage: $0 <source_file.c> <output_file>"
    exit 1
fi

mpicc -Wall -o "$2.o" "$1"
