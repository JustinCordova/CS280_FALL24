#!/bin/sh

# Check if a program number is provided as an argument
if [[ $# -ne 1 ]]; then
    echo "Usage: $0 <program_number>"
    exit 1
fi

# Define the program number based on the argument passed
PROG_NUMBER=$1

# Define the source files and output executable
SOURCE_FILES=(prog2.cpp parser.cpp lex.cpp)  # You can adjust this to include the correct prog files
OUTPUT_EXE="PA2"

# Compile the code
clang++ -std=c++17 -o $OUTPUT_EXE "${SOURCE_FILES[@]}"

# Check if compilation was successful
if [[ $? -ne 0 ]]; then
    echo "Compilation failed."
    exit 1
fi

# Define the test file and corresponding correct file based on the program number
TEST_FILE="PA2TC/testprog${PROG_NUMBER}"
CORRECT_FILE="PA2TC/testprog${PROG_NUMBER}.correct"

# Check if the test file and correct file exist
if [[ ! -f "$TEST_FILE" ]]; then
    echo "Test file $TEST_FILE not found."
    exit 1
fi

if [[ ! -f "$CORRECT_FILE" ]]; then
    echo "Correct output file $CORRECT_FILE not found."
    exit 1
fi

# Run the program with the test input file and capture the output
./$OUTPUT_EXE "$TEST_FILE" > "$TEST_FILE.output"

# Output the correct file content
echo "Correct Output:"
cat "$CORRECT_FILE"
echo "****************************************************************"

# Output the program's output
echo "Program Output:"
cat "$TEST_FILE.output"

# Compare the output with the expected output file
if cmp -s "$TEST_FILE.output" "$CORRECT_FILE"; then
    echo "$TEST_FILE: Output matches expected."
else
    echo "$TEST_FILE: Output does not match expected."
fi

# Optional: Clean up the output file
rm "$TEST_FILE.output"
