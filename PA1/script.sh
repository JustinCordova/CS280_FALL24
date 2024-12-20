#!/bin/bash

# Compile source files with clang++ using C++17 standard
clang++ -std=c++17 -c lex.cpp -o lex.o
clang++ -std=c++17 -c main.cpp -o main.o

# Link object files to create the final executable
clang++ -std=c++17 -o test lex.o main.o

# Change directory to PA1TC
cd PA1TC || exit

# Specify the test file and the correct output file
test_file="prog1"          # The test file to run
correct_file="prog1.correct" # The expected correct output file

# Run the test file with -all -num arguments and capture the output
echo "Running $test_file with arguments -all -num..."
output=$(../test "$test_file" -all -num)  # Capture the output in a variable
echo "$output"  # Print the original output to the terminal

# Save the output to a temporary file for comparison
echo "$output" > output.txt

# Compare the output against the correct file and show differences with !
if diff -q output.txt "$correct_file" > /dev/null; then
    echo "Test passed: Output matches $correct_file"
else
    echo "Test failed: Output does not match $correct_file"
    echo "Differences:"
    # Show differences with '!' instead of '>' and '<'
    diff -u output.txt "$correct_file" | sed 's/^+/! /; s/^-/! /' | grep '!'
fi

# Clean up temporary output file
rm output.txt
