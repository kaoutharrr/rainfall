
## Vulnerability Overview

**Type:** Use-After-Free (Dangling Pointer) + Heap Buffer Overflow

**Core Issue:** The program fails to nullify the global `auth` pointer after freeing it via the `reset` command. Furthermore, the `service` command writes user input into the heap without safe boundary checks, allowing a heap overflow to reach the stale `auth` structure's offset.

---

## Program Structure

The program acts as an interactive command loop managing global heap pointers:
- `auth <string>` → Allocates memory for the authentication structure.
- `reset` → Frees the `auth` allocation but leaves the pointer dangling (pointing to the freed address).
- `service <string>` → Allocates a new buffer on the heap for a service string.
- `login` → Evaluates if `auth[32]` (offset 32 from the `auth` pointer) is non-zero. If true, it executes `system("/bin/sh")`.

---

## Exploitation Strategy

1. **Trigger UAF:** Calling `auth` followed by `reset` frees the heap chunk but leaves the global `auth` pointer active and pointing to its original address (`0x804a008`).
2. **Heap Alignment:** When `service` is called, `malloc` hands back a chunk that overlaps/sits right inside the footprint of the old `auth` structure. Based on GDB analysis, this new buffer is allocated at `0x804a018`—exactly 16 bytes into the `auth` structure's footprint.
3. **The Overflow:** Because the target authentication status integer sits at `auth + 32` (`0x804a028`), and our `service` buffer starts 16 bytes into that footprint, we must write a string longer than 16 bytes to reach it ($0x804a028 - 0x804a018 = 16\text{ bytes}$). 
4. **Corrupting the Value:** Writing beyond 16 bytes causes the string to overflow past the `service` buffer boundary, spilling forward through memory to overwrite the `auth->status` integer slot. This forces the evaluation value to become non-zero.

---

## Solution

```bash
./level8
auth AAAA
reset
service sssssssssssssssssssssssssssssxxx
login
cat /home/user/level9/.pass