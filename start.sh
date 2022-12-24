#!/bin/bash

# Start the first player in a separate process.
./player &

# Start the second player in a separate process.
./player &

# Wait for the processes to finish.
wait