

## Vulnerability Overview

**Type:** Heap buffer overflow + GOT (Global Offset Table) overwrite

**Core Issue:** The program uses `strcpy()` to copy user-supplied arguments into small (8-byte) heap-allocated buffers without any size checking.

---

## Program Structure


**`m()`** - The target function (never called in normal execution)

This prints a global variable `c` (which contains the password) along with the current time.

**`main()`** - Contains two dangerous `strcpy` calls:

### Heap Layout

The program allocates 4 chunks via `malloc(8)`:
```
ptr1 = {int a=1, void *b=buffer1}
buffer1 = 8-byte buffer (destination of first strcpy)

ptr2 = {int a=2, void *b=buffer2}
buffer2 = 8-byte buffer (destination of second strcpy)
```
**Memory layout (discovered via GDB):**
```
0x804a018: buffer1 (8 bytes, initially empty)
0x804a028: ptr2->a = 2
0x804a02c: ptr2->b = pointer to buffer2
```
---

## Exploitation Strategy

### Step 1: Understanding Heap Chunk Sizing

Although we request 8 bytes per buffer, glibc's malloc allocates in 16-byte minimum chunks (for alignment and metadata). This means our small buffers sit directly adjacent to the next chunk's metadata in memory.

### Step 2: Calculate Overflow Offset

The distance from buffer1's start to ptr2->b:
```
0x804a02c - 0x804a018 = 0x14 = 20 bytes (decimal)
```

So writing 20 bytes of data into buffer1 reaches exactly to ptr2->b.

### Step 3: GOT Overwrite Mechanism

Instead of letting ptr2->b point to its normal buffer, we overwrite it with the address of `puts`' GOT entry:
```
puts GOT entry address: 0x08049928
m function address:     0x080484f4
```

### Step 4: The Exploit Chain

**argv[1]:** 20 bytes of padding + puts' GOT address (little-endian)
```
'A'*20 + '\x28\x99\x04\x08'
```
Result: First `strcpy` overflows buffer1, overwrites ptr2->b to point to puts' GOT entry.

**argv[2]:** m's address (little-endian)
```
'\xf4\x84\x04\x08'
```
Result: Second `strcpy` writes into the location pointed to by ptr2->b (the GOT entry), replacing puts' address with m's address.

---

## Exploitation

### Command
```bash
./level7 $(python -c "print 'A'*20 + '\x28\x99\x04\x08'") $(python -c "print '\xf4\x84\x04\x08'")
```

### What Happens

1. First `strcpy(buffer1, argv[1])` overflows buffer1 and lands on ptr2->b
2. ptr2->b is overwritten with 0x08049928 (puts' GOT address)
3. Second `strcpy(ptr2->b, argv[2])` writes m's address INTO the GOT
4. Later, when `main()` calls `puts()`, the CPU reads the GOT entry
5. Instead of jumping to real `puts()`, it jumps to `m()`
6. `m()` executes and prints the password

---

## Key Concepts

### Heap Allocation & Metadata
- `malloc(8)` doesn't actually give 8 bytes; glibc rounds up to minimum chunk size (16 bytes)
- Each chunk has a hidden size field (e.g., 0x00000011) before the usable data
- Overflowing a buffer into this metadata causes malloc corruption

### GOT Overwrite
- The GOT is a table of function addresses loaded at runtime
- PLT stubs read the GOT to determine where to jump
- By overwriting a GOT entry, we redirect ALL calls to that function
- This is a common exploitation technique when we can write to memory

### Little-Endian Encoding
- x86 architecture stores multi-byte values with the least significant byte first
- Address 0x08049928 becomes \x28\x99\x04\x08 in memory
- This is why we had to reverse the byte order in our payloads

### Timing
- We chose `puts()` because it's called at the END of main, after the password is already loaded into `c`
- If we'd hijacked an earlier function, `c` would still be empty

## Learning Outcomes

1. **Buffer overflow exploitation** - Understanding how overflowing small buffers can reach adjacent heap chunks
2. **GOT overwrite technique** - Redirecting function calls by modifying the Global Offset Table
3. **Heap layout analysis** - Using GDB to map heap memory and calculate precise offsets
4. **Little-endian encoding** - Converting addresses to the byte order required by x86
