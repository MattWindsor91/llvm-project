//
// Created by Matthew Windsor on 15/01/2021.
//

#ifndef LLVM_MUTATION_H
#define LLVM_MUTATION_H

#include <cstdint>
#include <iostream>

namespace llvm {

// Number of memory orders in LLVM, minus consume.
// Ideally wouldn't be hardcoded, but we can't depend on AtomicOrdering here.
constexpr uint16_t MutableMemOrders = 7;
constexpr uint16_t MemOrderEntries = MutableMemOrders * MutableMemOrders;

enum class Mutation : std::uint16_t {
  None = 0,

  // One mutation each for every possible bitflip in the IsStrongerThan table.
  FlipIsStrongerThan,
  // One sub-mutation for each pair of (viable) memory orders.
  EndFlipIsStrongerThan = FlipIsStrongerThan + MemOrderEntries - 1,

  // As above but for IsAtLeastOrStrongerThan.
  FlipIsAtLeastOrStrongerThan,
  // One sub-mutation for each pair of (viable) memory orders.
  EndFlipIsAtLeastOrStrongerThan = FlipIsAtLeastOrStrongerThan + MemOrderEntries - 1,

  AArch64ExpandCmpXchgO0ToLLSC,
  ARMExpandCmpXchgO0ToLLSC,
  Count,
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
const std::string_view MutationName(Mutation m);
}

#endif // LLVM_MUTATION_H
