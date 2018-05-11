# Lecture 4 - Assembly Language

## Template for Procedure


```
# Backup registers used in the procedure into memory
sw $2, -4($30)
sw $3, -8($30)

# Decrement $30
lis $3
.word 8
sub $30, $30, $3

# Code body ...

# Increment $30
add $30, $30, $3

# Recover registers
lw $3, -8($30)
lw $2, -4($30)
```

### Call
```
main:

lis $5
.word f # Address of the line labelled f
jr $5   # Jump to that line


f: # Address of this line in RAM

```

### Return
Need to set `PC` to the line after the `jr`.

How do we know which address that b is?  
Solution: `jalr` instruction "jump and link to register".

- Like `jr`, but sets `$31` to the address of the next instruction. (i.e., to `PC`).

So replace `jr $5` above with `jalr $5`. Then return `jr $31`.

Q: `jalr` overwrites `$31`, then how can we return to the loader? And what if `f` calls `g`?

A: Need to save `$31` on the stack first and restore when the call returns.

### Correct template:

```
main:

# Load the address of f into $5
lis $5
.word f

# Backup $31 on the stack
sw $31, -4($30)

# Decrement stack ptr
lis $31
.word 4
sub $30, $30, $31

# Call f
jalr $5

# Increment stack ptr
lis $31
.word 4
add $30, $30, $31

# Restore $31
lw $31, -4($30)

...

jr $31


f:

...

jr $31 # Return to the caller
```

### Param/Result Passing

Generally, need to use registers. Document the code. If too many params, can push params onto the stack.

#### Example
Sum 1 to N.  

Registers:

- `$1` for working (Save to RAM)
- `$2` for input (value of N, save to RAM)
- `$3` for output

```
# Backup registers
sw $1, -4($30)
sw $2, -8($30)

# Decrement stack pointer
lis $1
.word 8
sub $30, $30, $1

top:
add $3, $0, $0
add $3, $3, $2
lis $1
.word 1
sub $2, $2, $1
bne $2, $0, top

# Increment the stack pointer
lis $1
.word 8
add $30, $30, $1

# Restore registers
lw $2, -8($30)
lw $1, -4($30)
jr $31

```

## Recursion

No extra machinery needed. If registers, params, stack are managed properly, recursion will just work.

## I/O

### Output

Use `sw` to store a word at location `0Xffff000c`. Least significant (last) byte will be printed.

#### Example

Print "CS" follow by a newline

```
# Load the address into $1
lis $1
word 0xffff000c

# C
lis $2
.word 67
sw $2, 0($1)

# S
lis $2
.word 83
sw $2, 0($1)

# Newline
lis $2
.word 10
sw $2, 0($1)
```


### Input

Load a word from `0xffff0004`. The least significant byte is the character that was read.

If EOF, get -1 (i.e., 1111 ... 1)


# The Assembler

The assembler turns code that human can read to code that machine can read.  

Assembly code => Assebler => Machine code 

Any translation process involves two phases:

- Analysis - understand what os meant to be the source string.
	1. Group characters into meaningful tokens, e.g., label, hex #, register #, .world, etc. (This is done for you in starter code.)
	2. Group tokens into instructions (if possible).
- Synthesis - output the equivalent target string.
	1. Output equivalent machine code.

If the source is not valid assembly code, output error to stderr.

Advice: there are more wrong token configurations then right ones. Focus on finding all the right ones, anything else is error.

We will talking about parsing later.
