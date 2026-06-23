# rainfall
gdb : It is a command-line tool used to inspect and control what a program is doing while it executes.
why get is dangerous: gets() reads bytes from stdin one by one and writes them into the destination buffer. It stops only when it sees a newline (\n) or EOF. It has no length parameter — it literally cannot know how big the buffer is.

lea: Stands for Load Effective Address. It is primarily used to calculate addresses without ever interacting with actual memory.

RET is just one instruction that does this:
pop the top of the stack → put it in EIP

┌──────────────────┐
│  return address  │  ← "go back here when main() is done"
├──────────────────┤
│  saved EBP       │
├──────────────────┤
│  buffer[64]      │
└──────────────────┘