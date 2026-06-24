
## Vulnerability Overview

**Type:** Logic vulnerability (atoi + strcmp bypass)

**Core Issue:** The program reads a password from a file, inserts a null byte 
at index `atoi(argv[1])`, then compares the buffer with argv[1] using strcmp.
Passing an empty string bypasses the password check entirely.


## Exploitation Strategy

- `buf` stores the password read from file
- The program converts argv[1] to integer via `atoi(argv[1])`
- It then does `buf[idx] = '\0'` to truncate the buffer at that index
- `strcmp` compares the truncated buffer with argv[1]

## Why Empty String Works

argv[1] = ""
atoi("") = 0
buf[0] = '\0'       ← password truncated to empty string
strcmp("", "") = 0  ← both empty strings are equal!
execl("/bin/sh")    ← shell spawns!

## Solution

```bash
./bonus3 ""
cat /home/user/end/.pass
```