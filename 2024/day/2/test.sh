#!/bin/bash

TARGET=$1

RESULT=$(cat example_input.txt | ./$TARGET)
EXPECTED=$(cat example_output_$TARGET.txt)

if [ "$RESULT" == "$EXPECTED" ]; then
	echo "Test passed"
	exit 0
else
	echo "Test failed"
	echo "Expected: $EXPECTED"
	echo "Got: $RESULT"
	exit 1
fi

