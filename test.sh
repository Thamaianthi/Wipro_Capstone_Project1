#!/bin/bash

# Define the number of clients to run
num_clients=50

# Loop to run clients
for ((i=1; i<=$num_clients; i++))
do
    # Command to run your client, replace this with your actual client command
    # Example: ./client_program &
    ./your_client_program &

    # Optional: Add sleep to stagger the start of each client
    # sleep 1
done

# Optional: Wait for all clients to finish
wait