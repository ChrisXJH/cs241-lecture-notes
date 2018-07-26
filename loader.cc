

// Loader relocation algorithm

void loaderRelocation() {
    read();
    endMod = read();
    endCode = read() - 12;
    alpha = findFreeRAM();
    for (int i = 0; i < codeLength; i += 4) {
        MEM[alpha + i] = read();
    }
    while (i < endMod) {
        format = read();
        if (format == 1) {
            rel = read();
            MEM[rel + alpha - 12] += alpha - 12;
        }
    }
    return 0;
}


/**
*
* Linkers
*
* Convenient to split large MIPS program into small units.
*
* Issue: how can a assemble assemble a reference to a lable if it's in another file.
*
* Solution 1:
*      Concatenate the asm files together and assemble the result.
*      i.e., cat a.asm b.asm c.asm | cs241.binasm
*      Works... but want to share binary files only.
*
*      Can we assemble first and then cat?
*          The assembly output has to be relocatable. At least one of the pieces can be loaded
*          at address 0. So output mush be MERL not MIPS. But we cannot cat two MERL files
*          (not MERL anymore.)
*
* Solution 2:
*      We meed a tool that understands MERL and can put object files together correctly.
*      i.e., a linker.
*      What should an assembler do with references to understand labels?
*          Need to change the assembler. When the assembler encounters .word ID where ID is not
*          found, put 0 in place of the label reference.
*          The assembler mush indicate the program needs the value of ID before it can run.
*
*      How does the assembler indicate it needs a value for an ID?
*          Make an entry in the MERL file.
*
*      But don't we lood a valuable error check? What if the ID is really missing?
*      Assembler will ask for the ID to be linked, instead of signalling the error.
*          New assembler directive: .import ID
*          This tells the assembler ID was meant to be linked in. This directive will not be
*          assembled to binary.
*
*
*      So, when an assembler sees .word ID, if ID is not in current file and no .import ID, then error.
*
*      MERL entry: External Symbol Reference (SER)
*          format code: 0x11
*
*          Information needed:
*              Line at which the symbol is being used.
*              Name of the symbol
*
*          Format:
*              word1: 0x11 (this is the format code)
*              word2: location where the label is used
*              word3: length of the name in characters
*              word4 - word(3 + length): ASCII characters in the symbols name
*
*
*      MERL for the program on the other side that provides the symbol:
*          Duplicated labels may happen. Need to restrict the set of labels that are
*          available for linking.
*          New directive: .export ID. This makes ID available for linking. Does not assemble to MIPS.
*          The assembler should make a MERL entry for ID exported.
*
*      MERL entry: External Symbol Definition (ESD)
*          format code: 0x05
*
*      Format:
*          word1: 0x05 (the formal code)
*          word2: address the symbol represents
*          word3: length of the name in characters
*          word4 - word(3 + length): ASCII characters in the symbols name
*
*
*      MERL file now contains:
*          the code
*          all addresses for relocation
*          addressses and names for ESR
*          addressses and names for ESD
*/

// Linker algorithm

void link(MERL m1, MERL m2) {
    alpha = codeLength(m1) - 12;
    relocate(m2, alpha); // relocate m2 by alpha
    if (intersect(m1.exports.labels, m2.export2.labels) != EMPTY) throw Exception{"ERROR"};
    for (PAIRr<addr_1, label> pair : m1.imports) {
        if (in(pair<addr_2, label>, m2.exports)) {
            m1.code[addr_1] = addr_2;
            remove(m1.import2, pair<addr_1, label>);

            // Need to add it to the relocation entries
            add(addr_1, m1.relocationEntries);
        }
    }

    // Do the same thing for m2
    for (PAIRr<addr_2, label> pair : m2.imports) {
        if (in(pair<addr_1, label>, m1.exports)) {
            m2.code[addr_2] = addr_1;
            remove(m2.import2, pair<addr_2, label>);

            // Need to add it to the relocation entries
            add(addr_2, m2.relocationEntries);
        }
    }

    // Generate new imports, exports, and relocation entries.
    imports = union(m1.imports, m2.imports);
    exports = union(m1.exports, m2.exports);
    relocationEntries = union(m1.relocationEntries, m2.relocationEntries);

    mResult = new MERL(m1.code + m2.code, imports, exports, relocationEntries);
    return mResult;
}
