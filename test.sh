#!/bin/bash

cd benchmarks/groups/group/ || exit

printf '%s\n' "--------------------- Initiate tests --------------------------"
printf '%s\n' ""
printf '%s\n' "==============================================================="
printf '%s\n' "Running Benchmarks..."
printf '%s\n' "==============================================================="

printf "\n$>bench1.sh\n\n"
bash bench1.sh

printf "\n$>bench2.sh\n\n"
bash bench2.sh

printf "\n$>bench3.sh\n\n"
bash bench3.sh

printf '%s\n' ""
printf '%s\n' "--------------------- Finalize tests --------------------------"
printf '%s\n' "==============================================================="
