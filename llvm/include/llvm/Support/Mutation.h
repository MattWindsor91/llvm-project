//
// Created by Matthew Windsor on 15/01/2021.
//

#ifndef LLVM_MUTATION_H
#define LLVM_MUTATION_H

#include <cstdint>
#include <iostream>

namespace llvm {

enum class Mutation : std::uint16_t {
  None = 0,
  AArch64ExpandCmpXchgO0ToLLSC = 1,
  ARMExpandCmpXchgO0ToLLSC = 2,
  Count = 3,
};

extern Mutation C4Mutation;

// Main toggle for whether a mutation is hit.
inline bool C4Mut(Mutation m) {
  if (C4Mutation == m) {
    std::cerr << "MUTATION HIT: " << static_cast<std::uint16_t>(m) << std::endl;
      return true;
  }
  return false;
}

// As above but adding an integer offset into the table.
inline bool C4MutOffset(Mutation m, uint16_t off) {
  return C4Mut(static_cast<Mutation>(static_cast<std::uint16_t>(m) + off));
}

void SetupMutation();
}

#endif // LLVM_MUTATION_H
