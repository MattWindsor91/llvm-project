# Concurrency mutation testing using LLVM

This is an unofficial fork of [LLVM 11](https://github.com/llvm/llvm-project)
with hand-instrumented mutation operators targeting interesting decision points
in how LLVM handles atomic-action concurrency.  While each mutant targets LLVM,
Clang is the expected front-end, and other front-ends might not work properly.

_Do not send bug reports upstream to LLVM!_  Not only is this project entirely
unsupported by them, but it tracks a specific compiler version (which won't be
the most recent by the time you read this), and _deliberately induces bugs_ in
its own control flow (that's the point of it!).

## Building

Exactly as normal LLVM.

## Using the mutations

Mutations are specified at run-time by the `C4_MUTATION` variable, which takes
an integer.  Each number corresponds to a particular mutation in
`llvm/include/llvm/Support/Mutation.h`, and numbers in excess of the highest
mutation number will wrap around.  Mutations resolving to number 0 correspond
to disabling all mutants.

The compiler will register a selection by dumping
`MUTATION SELECTED: number (details)` to stderr.  It will then register
`MUTATION HIT: number` to stderr every time a control flow path affected by
that mutation is reached.  This can be used to confirm that the mutant was hit.
