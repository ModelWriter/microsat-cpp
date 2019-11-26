#!/bin/bash

printf '%s\n' "--------------------- Initiate tests --------------------------"
printf '%s\n' ""
printf '%s\n' "==============================================================="
printf '%s\n' "Running Given Sample Games and Comparing the results..."
printf '%s\n' "==============================================================="

printf "\n$>./kalah 6 4 human human < tests/sample.in\n\n"
valgrind --leak-check=full ./kalah 6 4 human human <tests/sample.in


printf '%s\n' ""
printf '%s\n' "--------------------- Finalize tests --------------------------"
printf '%s\n' "==============================================================="
