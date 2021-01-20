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

// Number of submutations for WeakenCABI: acquire, release, acq_rel, seq_cst.
constexpr uint16_t NumCABIWeakenings = 4;

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

  // Set acq/rel/acqrel/seq_cst (in that order) to relaxed in the C ABI mapping.
  WeakenCABI,
  // One sub-mutation for each memory order above.
  EndWeakenCABI = WeakenCABI + NumCABIWeakenings - 1,

  AArch64ExpandCmpXchgO0ToLLSC,
  ARMExpandCmpXchgO0ToLLSC,
  Count,
};

extern Mutation C4Mutation;

// Main toggle for whether a mutation is hit.
inline bool c4Mut(Mutation M) {
  if (C4Mutation == M) {
    std::cerr << "MUTATION HIT: " << static_cast<std::uint16_t>(M) << std::endl;
      return true;
  }
  return false;
}

// As above but adding an integer offset into the table.
inline bool c4MutOffset(Mutation M, uint16_t Offset) {
  return c4Mut(static_cast<Mutation>(static_cast<std::uint16_t>(M) + Offset));
}

void setupMutation();
const std::string_view mutationName(Mutation M);
}

#endif // LLVM_MUTATION_H
