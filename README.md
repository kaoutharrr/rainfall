# Rainfall

> A 42 School introductory binary exploitation / reverse engineering project.

## Introduction

**Rainfall** is one of 42's early security-focused projects, designed as a hands-on introduction to **binary exploitation** and **reverse engineering** on Linux. Students connect to a remote server hosting a series of small, deliberately vulnerable C binaries (`level0`, `level1`, `level2`, ...). Each level runs with elevated privileges (setuid), and the goal is to find a flaw in that level's binary that lets you read the password for the next level — effectively privilege-escalating your way up the chain.

Unlike most 42 projects, there's no fixed "correct" implementation to submit. Progress is measured by how many levels you successfully clear, making it closer to a **wargame / CTF (Capture The Flag)** format than a typical coursework project.

## Learning Objectives

Rainfall pushes students to get comfortable with the tools and mindset of low-level security analysis:

- **Reading and understanding compiled binaries** without source code, using disassemblers and debuggers.
- **Recognizing common vulnerability classes** in C programs — memory safety issues, unsafe input handling, and unintended trust boundaries.
- **Working with the Linux process model**, including how setuid binaries execute with the permissions of their owner rather than the invoking user.
- **Basic exploit development fundamentals** — understanding *why* a bug is exploitable conceptually (without this repo detailing *how* to weaponize any specific level).
- **Command-line and scripting fluency** under a constrained, minimal environment (often just a shell, GDB, and standard Linux tools — no fancy IDEs).

## Tools Typically Used

- **GDB** — for dynamic analysis: stepping through execution, inspecting memory/registers, setting breakpoints.
- **objdump / readelf** — for static inspection of ELF binaries, symbols, and sections.
- **ltrace / strace** — for observing library and system calls at runtime.
- **Python** (often with `pwntools`-style scripting patterns) — for automating interaction with a running binary once a vulnerability is understood.
