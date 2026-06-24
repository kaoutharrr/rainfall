# Bonus1 Exploit Explained Simply

## 1. The Strategy
The program reads an integer from the user and checks if it is less than 10 (`num < 10`). If it is, the check passes. 

Later, the program uses `memcpy` to copy data, but it determines the copy size by multiplying our input by 4 (`num * 4`). `memcpy` only understands **positive (unsigned)** values.

Our goal is to force `memcpy` to copy exactly **44 bytes** (40 bytes to completely fill the buffer + 4 bytes to overwrite the adjacent `num` variable).

---

## 2. The Integer Overflow Trick
To bypass the `< 10` check but still get a size of 44, we use the 32-bit minimum integer territory:

* **The Baseline:** The absolute minimum signed 32-bit integer is `-2147483648` (`0x80000000`). When multiplied by 4, it wraps around the 32-bit register completely and resets back to `0`. Because a 32-bit register can only hold exactly 8 hexadecimal digits and `min_int * 4` (`0x200000000`) contains 9 hexadecimal digits, the CPU ignores the leading `2` and stores `0x00000000`. It wraps to 0 and naturally passes any small-value verification.
* **The Shift:** Since we want 44 bytes, and $44 \div 4 = 11$, we shift exactly 11 steps forward from the minimum integer baseline:
  -2147483648 + 11 = -2147483637

When the program multiplies `-2147483637` by 4, the huge minimum integer chunk overflows and vanishes, leaving behind a clean, positive **44** for `memcpy`.

---

## 3. The Payload Structure
Because `memcpy` is forced to copy 44 bytes into a 40-byte box, the extra data spills over the edge.



* **`"A" * 40`**: Padding used to fill the destination buffer perfectly to the brim.
* **`"FLOW"`**: The exact 4-byte string required by the program's conditional check. Because of the overflow, these 4 bytes spill directly into the adjacent `num` variable, changing its value and triggering the shell!

---

## 4. Final Command

`./bonus1 -2147483637 $(python -c 'print("A"*40 + "FLOW")')`