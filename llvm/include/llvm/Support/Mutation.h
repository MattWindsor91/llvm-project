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

inline bool C4Mut(Mutation m) {
  if (C4Mutation == m) {
    std::cerr << "MUTATION HIT: " << static_cast<std::uint16_t>(m) << std::endl;
      return true;
  }
  return false;
}

void SetupMutation();
}

#endif // LLVM_MUTATION_H
