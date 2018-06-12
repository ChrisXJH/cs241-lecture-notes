# LL(1)
#### Example:
E -> E + T | T  
T -> T * F | F  
F -> a|b|c

Not LL(1), why?
E => E + T => T + T => a + T  
E => T => F => a  


Left recursion always not LL(1).

Need to make it right recursive:

E -> T + E | T  
T -> F * T | F  
F -> a|b|c

Still not LL(1) - T generates the same first symbols.

Factor:

E -> TE'  
E' -> epsilon | + e  
T -> FT'  
T' -> epsilon | * T  
F -> a|b|c


So LL(1) is at odds with left-associativity.

Bottom - up parsing

Stack stores partially-reduced into read so far.  
w <= a_k <= a_k-1 <= ... <= a_1 <= S

Invariant: stack + unread input = a_i (or w or s)

#### Example
S' -> BOF S EOF  
S -> AyB  
A -> ab  
A -> cd  
B -> z  
B -> wx  

| Stack        | Read Input           | Unread  | Action |
| ------------- |:-------------| -----:|-------------|
|  empty     | epsilon| BOFabywxEOF | shift BOF|
|BOF     | BOF      |   abywxEOF |shift a|
| BOF a | BOF a    |   bywxEOF  | shift b |
| BOF ab | BOF ab    |   ywxEOF  | reduce A->ab; pop b, pop a, push A |
| BOF A | BOF ab    |   wxEOF  | shift y |
| BOF Ay | BOF aby   |   wxEOF  | shift w |
| BOF Ayw | BOF abyw   |   xEOF  | shift x |
| BOF Aywx | BOF abywx   |   EOF  | reduce B->wx, pop w,x, push B |
| BOF AyB | BOF abywx   |   EOF  | reduce S -> AyB, pop AyB, push S |
| BOF  | BOF abywx   |   EOF  | reduce S -> AyB, pop AyB, push S |
| BOF S | BOF abywx   |   EOF  | shift EOF |
| BOF S EOF | BOF abywx EOF   |   epsilon  | reduce S' -> BOF S EOF, pop EOF, S, BOF, push S' |
| S' | BOF abywx EOF   |   epsilon  | Accept|

How do we know whether to shift or reduce?  
We use the next char to help decide.

### Theorem (Donald Knuth, 1965)
The set { wa | there exists x such that S => wax }  
w: stack  
a: next input character  

Set of stacks is a regular language:

- can be described by a DFA
- use the DFA to make shift/reduce decisions

Resulting method called LR parsing

- left-to-right scan
- rightmost derivation
