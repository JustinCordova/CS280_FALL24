#!/bin/shwhi

# Create a temporary file for capturing output
ofile=$(mktemp)

# Compile the code
clang++ -std=c++17 -o PA2 prog3.cpp parserInterp.cpp val.cpp lex.cpp
if [ $? -ne 0 ]; then
    echo "RESULT: compiles [ 0 / 1 ]"
    exit 1
fi

echo "RESULT: compiles [ 1 / 1 ]"

# Initialize total tests and passed tests
total_tests=20
passed_tests=0

# Loop through all test files in PA2TC
for i in {1..19}; do
    TEST_FILE="PA3TC/testprog$i"
    CORRECT_FILE="PA3TC/testprog$i.correct"

    # Check if the test file and correct file exist
    if [[ ! -f "$TEST_FILE" ]]; then
        echo "Test file $TEST_FILE not found."
        echo "RESULT: testprog$i [ 0 / 1 ]"
        continue
    fi

    if [[ ! -f "$CORRECT_FILE" ]]; then
        echo "Correct output file $CORRECT_FILE not found."
        echo "RESULT: testprog$i [ 0 / 1 ]"
        continue
    fi

    # Run the program with the test input file and capture the output
    ./PA2 "$TEST_FILE" > "$TEST_FILE.output"

    # Compare the output with the expected output file
    if cmp -s "$TEST_FILE.output" "$CORRECT_FILE"; then
        echo "RESULT: testprog$i [ 1 / 1 ]"  # Passed
        ((passed_tests++))
    else
        echo "RESULT: testprog$i [ 0 / 1 ]"  # Failed
    fi

    # Optional: Clean up the output file
    rm "$TEST_FILE.output"
done

# Print final summary
echo
echo SUMMARY:
echo "RESULT: total [ $passed_tests / $total_tests ]"
