# Lecture 3
## Assembly Language
Replacing tedious binary/hex encodings with easier-to-use mnemonics.  

- Less change of error.
- Translation to binary can be automated (assembler).
- One line of assembly equals one word on the machine.

#### Example 1
Put `42 + 52` in `$3`.

```
lis $5				#load immediate and skip
word 42				#not an insertion
lis $7					
.word 52
add $3, $5, $7		#destination goes first
jr $31
```

#### Example 2
Compute the absolute value of `$1`, store in `$1` and return.

- 	Some instructions modify PC - "branches & jump". e.g., `jr`.
-  `beq` branch if two registers have equal contents.
	- Increment PC by given number of words.
	- Can be branch backwark.
- `bne` - "branch not equal".
- `slt` - "Set less than".
	- ```slt $a, $b, $c - $a <= 1 if $b < $a```

```
00000000| slt $2, $1, $0
00000004| beq $2, $0, 1
00000008| sub $1, $0, $1
0000000C| jr $31
```

#### Example 3
Sum the integers 1, ... ,13, store in `$3`, and return.

```
00000000| lis $2
00000004| .word 13
00000008| add $3, $0, $0
0000000C| lis $1
00000000| .word 1
00000004| sub $2, $2, $1
00000008| bne $2, $0, -5
0000000C| jr $31
```

### Using Lables

Q: What if an instruction is added or removed in between?  
A: Need to update branch offset (i.e., `bne $2, $0, -5`). Updating all branch offsets is hard. Instead, the assembler allowso labelled instructions. E.g., label: operation operands; comment. Start: `add $3, $3, $0`.

- Assembler associates the name "start" with the address of `add $3, $3, $0` in the binary.

Rework the above example:  

```
00000000| lis $2
00000004| .word 13
00000008| add $3, $0, $0
top: (top = 0X0C)
0000000C| lis $1
00000000| .word 1
00000004| sub $2, $2, $1
00000008| bne $2, $0, top		#assembler computes (top - PC)/4
0000000C| jr $31
```


## RAM
lw - "load word" from RAM into a register.  

- `lw $a, i($b)`
- `$a <= MEM[$b + i]`

sw - "store word" from a register into RAM

#### Example 4
`$1` is the address of an array. `$2` is the number of elements in the array. Place the 5th element in `$3`.

Easy way:  

```
lw $3, 20($1)  
jr $31  
```

Hard way (assuming we don't know it's 5 in advance):  

```
00000000| lis $5
00000004| .word 5		#or whatever
00000008| lis $4
0000000C| .word 4
00000000| mult $5, $4
00000004| mflo $5
00000008| add $5, $5, $1
0000000C| lw $3, 0($5)
00000000| jr $31
```


Note: `mult $a, $b` - "multiply". Product of a 32-bit numbers is 64-bit. Too big for a registry. So two special registers hi and lo store the results of `mult`. `mult $a, $b`: `hi:lo <= $a * $b`

- `mflo` - "move from lo"
- `mfhi` - "move from hi"

For devision, lo = quotient, hi = remainder.

## Procedures

Want to write a procedure `f` in MIPS assembly.  

Two problems to be solved:

1. Call and return
	- How do we transfer control into & out of the procedure f?
	- What if `f` calls `g`?
	- How do we pass params?
2. Registers
	- What if `f` overwrites a register we are using?
	- Corruptions

For problem 2, we could reserve some registers for `f` and some for main, so that they won't interfere with each other. But what if `f` calls `g`? We will run out of registers since there are only 32 registers.  

Instead, we could guarentee that procedures leave registers unchanged when done. How?

### Use RAM to keep register values unchanegd

We can use RAM to keep register values unchanegd. But how do we keep procedures from using the same RAM?  

Two sensable options:

- Allocate memory from either top or bottom of free RAM.
- Need to keep track of which RAM is in use and which isn't.

MIPS machine helps us out. `$30` is initialized by the loader to just past the last used word of memory. Can use `$30` as a bookmark to separate used and unused RAM if we allocation rom the bottom.

`$30` - the stack pointer which contains address of top of stack.

Eg: Say `f` calls `g`, `g` calls `h`.  

Memory diagram:  

Memory |
---|
  |
  |
  |
  |
  |
h stored regs ($30)|
g stored regs|
f stored regs|
