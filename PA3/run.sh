#!/bin/bash

# Check if a program number is provided as an argument
if [[ $# -ne 1 ]]; then
    echo "Usage: $0 <program_number>"
    exit 1
fi

# Define the program number based on the argument passed
PROG_NUMBER=$1

# Define the source files and output executable
SOURCE_FILES=("prog3.cpp" "parserInterp.cpp" "val.cpp" "lex.cpp")  # Adjust as needed
OUTPUT_EXE="PA3"

# Compile the code
clang++ -std=c++17 -o "$OUTPUT_EXE" "${SOURCE_FILES[@]}"

# Check if compilation was successful
if [[ $? -ne 0 ]]; then
    echo "Compilation failed."
    exit 1
fi

# Define the test file and corresponding correct file based on the program number
TEST_FILE="TC/testprog${PROG_NUMBER}"
CORRECT_FILE="TC/testprog${PROG_NUMBER}.correct"
OUTPUT_FILE="${TEST_FILE}.output"

# Check if the test file and correct file exist
if [[ ! -f "$TEST_FILE" ]]; then
    echo "Error: Test file $TEST_FILE not found."
    exit 1
fi

if [[ ! -f "$CORRECT_FILE" ]]; then
    echo "Error: Correct output file $CORRECT_FILE not found."
    exit 1
fi

# Run the program with the test input file and capture the output
./"$OUTPUT_EXE" "$TEST_FILE" > "$OUTPUT_FILE"

# Output the correct file content
echo "Correct Output:"
cat "$CORRECT_FILE"
echo "****************************************************************"

# Output the program's output
echo "Program Output:"
cat "$OUTPUT_FILE"
echo "****************************************************************"

# Compare the output with the expected output file
if diff -q "$OUTPUT_FILE" "$CORRECT_FILE" > /dev/null; then
    echo "$TEST_FILE: Output matches expected."
else
    echo "$TEST_FILE: Output does not match expected."
    echo "Differences:"
    diff "$OUTPUT_FILE" "$CORRECT_FILE"
fi

# Optional: Clean up the output file
rm -f "$OUTPUT_FILE"
