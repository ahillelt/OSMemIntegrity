# OSMemIntegrity
Priority Based Parity Bit Assignment for Memory Integrity


This is a single-bit bit-flip detection algorithm that uses a parity bit.
No correction is possible at present.

This is a simulation program to run multiple test runs.
The user can define the word size in bits, probability of error as a decimal value, and the percentage allocation between priority and non priority.

Please note all allocations are a weighted random number that will approach the selected probability value as n increases. 

The scheduler reviews the priority or non priority flag to assign a parity bit and perform parity checks

A report is output as a CSV.

Alon Hillel-Tuch 7/2/2021

*Note: Simulation assumes that the system running the program has a zero error probability of 1, this is very unlikely.
