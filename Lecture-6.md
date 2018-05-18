## Finite Languages
Finite languages

- have only finitely many words
- can recognize as a word by comparing
- write each word in the finite set

Can we do this more efficiently?

Exercise: L = {cat, car, cow}

Write code to answer w ∈ L, such that w is scanned exactly once, without storing previously - seen characters.

Scan input left to right.  

```
if the first character is c, move on, else error.  
if the next char is a, move on, else error.  
If the next char is t
	If the next char is empty, move on, else error.
If the next char is r 
	If the next char is empty, move on, else error.
If the next char is w  
	If the next char is empty, move on, else error.
```
Since programming languages don't usually have only finit many programs, finit languages are of limited use.

### Regular Languages

Regular languages are

- build from union/concartenation, repetition of finite laguages

Repetition:
- L* = {Ɛ} U {xy | x ∈ L*, y ∈ L}  
     = {Ɛ} U L U LL U LLL  
     = 0 or more occurrences of a word in L  
  
 Show {a^(2n)b | n >= is regular}
 
 
({a,a})^*{b}

Is C regular?

A C program is a sequence of c tokens.

- numbers (0|...|9)(0|...|9)*
- keywords
- variables [a-zA-Z][a-zA-Z0-9]*

Sequence of c tokens come from regular language, therefore, C is a subset of valid C tokens. So, maybe.

How can we recognize an arbitrary regular language automaticallly.

Eg, {a^(2n)b | n >= 0}

Yes, if we allow loops in the diagram.

State diagrams are called Deterministic Finite Automata (DFAs)

- always start at the start state
- for each char in the input, follow the corresponding  arc to the next state.
- if an accepting state when input is exhausted, accept, else reject
- falling off the machine mush be reject

```
->() -a-> ()
  |\_______|
  b
  |
 (()) 
```


#### Example

Strings over {a,b} with an even number of a's and an odd number of b's.

```

                                 
```

Formal definition of a DFA

A DFA is a 5-tuple (Σ, Q, q_0, A, S, δ), where

- Σ is a finite non-empty set (alphabet)
- Q is a finite non-empty set (states)
- q_0 ∈ Q (start state)
- A ∈ Q ⊆ Q (accepting states)
- δ: Q X Σ -> Q (transition function)

δ consumes a single char - can extend δ to a function that consumes an entire word.

#### Definition

δ*(q, Σ) = q
