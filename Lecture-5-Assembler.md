# The Assembler
```
beq $0, $1, abc
```

Can't assemble the `beq` because we don't (yet) know what `abc` is.

Standard solution: assemble in two passes:

- Pass1:
	1. group tokens into instructions.
	2. record addresses of all lablled instructions.
		- "symbol table" - list of (lable, address) pairs

Note: a line of assembly can have more than one lable. E.g.,

```
f:
g: h:
mult $1, $2
```

- Pass2: 
	1. translate each instruction to machine code
	2. if an instruction refers to a lable, look up the the assembled address in the symbol.

Our assembler outputs the assembled MIPS to stdout, and output the symbol to stderr.

#### Example

```
main:	lis $2
		.word 13
		add $3, $0, $0
top:	
		add $3, $3, $2
		lis $1
		.word 1
		sub $2, $3, $1
		bne $2, $0, top
		jr $31
beyond:
```

Pass1: group tokens into instructions, build symbol table:

| Label        | Address           |
| ------------- |-------------|
| main      | 0x00 |
| top     |0x0c      |
|  beyond | 0x24     |

Pass2: translate each instruction

e.g., 

```
lis $2   => 0x00001014
.word 13 => 0x0000000d

...

bne $2, $0, top (look up top in symbol: 0x0c, (top - pc)/4) => 0x1440fffb
```

## Bit-level Operations

To assemble `bne $3, $0, top`, 

opcode = `000101`  
first reg = `$2`  
second reg = `$0`  
offset = `-5`  

To put `000101` in the first 6 bits, need to append 26 0's. ie, left shift by 26 bits.

=> `0001 0100 0000 0000 0000 0000 0000 0000`

Move `00010` 21 bits to the left.

=> `0 0010 0000 0000 0000 0000 0000`

Move `0` 16 bits to the left

=> `0 0000 0000 0000 0000 0000`

`-5 = 0xfffffffb` want only the last 16 bits

So do a bitwise "and" with `0xffff`: `-5 & 0xffff`

Lastly, use bitwise "or" to combine everything together:

```
unsigned int instr = (5 << 26) | (2 << 21) | (0 << 16) | (-5 & 0xffff);
cout << instr; // wrong answer
```

```
int n = 65;
char c = 65;
cout << n << c; // outputs 65A
```
Char output sends bytes as is int output converts digits to ASCII.

```
unsigned int instr = (5 << 26) | (2 << 21) | (0 << 16) | (-5 & 0xffff);
char c;
c = instr >> 24;
cout << c;
c = instr >> 16
cout << c;
c = instr >> 8;
cout << c;
c = instr;
cout << c;
```

## Formal Languages

High-level language => Compiler => Assembly language

Assembly language: 

- Simple structure
- Easy to recognize(parse)
- Straightforward, anambiguous translation to machine language

High-level language

- More complex structure
- Harder to recognize
- Usually no single translation to machine language

How can we handle the complexity?

Want a formal theory of string recognition

- General principles that work for any programming language.

#### Definition

Alphabet - finit non empty set of symbols. e.g., {a,b,c}

- Typically denoted as ∑.

String (or word) - finit sequence of symbols taken from the alpabet. e.g., "abc", "cbca".

- empty string - an empty sequence of symbols.

Length of a word - |w| = number of characters in w.

Language - set of strings. e.g., {a^(2n)b|n>=0}

Note: 

- Ɛ - empty word/
- {} or ∅ - language contains no word.
- {Ɛ} - singlton language that contains only Ɛ.

Question: how can we recognize automatically a given string belongs to a given language?

Answer: depends on how complex the language is. For example, 

- {a^(2n)b | n >= 0} - easy
- {valid MIPS assembly programs} - almost as easy
- {valid C programs} - harder
- Some languagues - impossible

Would like to characterize languages by how hard the recognition process is. Organize languages into classes of languages based on complexity of recognition.

- Finite
- Regular
- Context-free
- Context-sensitive
- Recursive
- etc.

(easy -> hard)