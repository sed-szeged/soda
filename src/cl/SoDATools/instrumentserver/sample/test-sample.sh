#!/bin/sh

g++ -o sample sample.cpp ../instrumenter/instrument.c -g -finstrument-functions -ldl
export INSTRUMENT=1
export TEST_NAME="sample-test-1"
./sample
export TEST_NAME="sample-test-2"
./sample 2
