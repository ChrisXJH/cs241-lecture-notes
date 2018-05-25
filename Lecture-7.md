# Lecture 7 - DFA
DFA Deinition:
M = (Σ, Q, q_0, A, S, δ) where M accepts w if δ*(q, Σ) ∈ A.

If M is a DFA, we denote by L(M) ("the language of M") the set of all strings accepted by M:

```
L(M) = {w | M accepts w}
```

#### Theorem (Kleene)
L is regular iff L=L(M) for some DFA M. (The regular languages are exactly the language accepted by DFA's).

Implementing DFA

```
int state = q_0
char ch
while not EOF do
	read ch
    case state of
    	q_0: case ch of
        	a: state = ...
            b: state = ...
        q_1: case ch of
        	a: state = ...
            b: state = ...
```

### DFA's with actions
Can attach computations to the arcs of a DFA.

#### Example
L = {binary numbers with no leading 0's}

### Non-Deterministic Finite Automata
What do we gain by making DFA's more complex?

#### Example
L = {w ∈ {a,b}* | w ends with abb}


What if we allowed more than one arc with the same label from the same state?

What would this mean? Machine chooses one direction or the other. Accept if some set of choices leads to accepting state.

Formally: An NFS is a 5-tuple (Σ, Q, q_0, A, δ) where
- Σ is a finite non-empty set (alphabet)
- Q: set of states
- q_0 ∈ Q (start)
- A is a subset of Q (accepting)
- δ: Q x Σ => subsets of Q 

What to accept if some parts through the NFA leads to acceptance for the given word (reject if none do)

δ* for NFA's: set of states x Σ => set of states

δ*(q_s, Σ) = q_s
δ*(q_s, cw) = δ*(Uδ*(q_s, c), w)

Then accept w if δ*({q_0}, w) n A != empty

NFA simulation procedure:

```
states <= {q_0}
while not EOF do
	ch <= read()
    states <= U δ*(q, ch)
end do
return states n A != empty
```
| Already read in       | Yet to be read           | State  |
| ------------- |:-------------:| -----:|
| empty    | baabb | {1} |
| b   | aabb      |   {1} |
| ba | abb    |    {1, 2} |
| baa | bb    |    {1, 2} |
| baab | b    |    {1, 3} |
| baabb | empty    |    {1, 4} |

If you give each set of states a name, and call these the states, even NFA becomes a DFA.
  