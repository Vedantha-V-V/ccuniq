# CCUNIQ

Lite version implementation of the Unix Command Line Tool uniq

## Project Link
[uniq Tool](https://codingchallenges.fyi/challenges/challenge-uniq/)

## Requirements
- gcc

## Running
```bash
gcc -o uniq ccuniq.c
./uniq [OPTIONS] [FILEPATH/s]
```

## Functionality
```bash
uniq [OPTION]... [INPUT [OUTPUT]]
Filters matching lines from INPUT (or standard input),writing to OUTPUT (or standard output).
OPTIONS:
With no FILE, or when FILE is -, read standard input
-c, --count show lines with number of occurences
-d, --repeated Only print duplicate lines, one each
-u Only print unique lines, one each
```