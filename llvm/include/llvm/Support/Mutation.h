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

// Number of submutations for MarkRMWIdempotent: add, sub, or, xor, and.
constexpr uint16_t NumRMWIdempotentCases = 5;

// Number of leading/trailing fence pairs: 0 in bracket, 1 and 2 in cmpxchg.
// 0 is important as it is the target of the SwapBracketFences mutation.
constexpr uint16_t NumLeadingTrailingFences = 3;

// Number of DMB making endpoints in ARMISelLowering.
constexpr uint16_t NumDMBs = 6;

enum class Mutation : std::uint16_t {
  None = 0,

  /*
   * AtomicOrdering
   */

  // One mutation each for every possible bitflip in the IsStrongerThan table.
  // see AtomicOrdering.h:102
  FlipIsStrongerThan,
  // One sub-mutation for each pair of (viable) memory orders.
  EndFlipIsStrongerThan = FlipIsStrongerThan + MemOrderEntries - 1,

  // As above but for IsAtLeastOrStrongerThan.
  // see AtomicOrdering.h:117
  FlipIsAtLeastOrStrongerThan,
  // One sub-mutation for each pair of (viable) memory orders.
  EndFlipIsAtLeastOrStrongerThan = FlipIsAtLeastOrStrongerThan + MemOrderEntries - 1,

  // Set acq/rel/acqrel/seq_cst (in that order) to relaxed in the C ABI mapping.
  // see AtomicOrdering.h:147
  WeakenCABI,
  // One sub-mutation for each memory order above.
  EndWeakenCABI = WeakenCABI + NumCABIWeakenings - 1,

  /*
   * AtomicExpandPass
   */

  // Spuriously mark a RMW as idempotent. (Likely only killed on x86)
  // see AtomicExpandPass.cpp:1360, 1361, 1362, 1364, 1366.
  MarkRMWIdempotent,
  // One sub-mutation for each case in the switch statement.
  EndMarkRMWIdempotent = MarkRMWIdempotent + NumRMWIdempotentCases - 1,

  // Change leading fence emissions to trailing.  (Likely only killed on arm32)
  // see AtomicExpandPass.cpp:318, 1210, 1232
  LeadingFenceIsTrailing,
  // One sub-mutation for each of the three insertions above.
  EndLeadingFenceIsTrailing = LeadingFenceIsTrailing + NumLeadingTrailingFences - 1,

  // Change trailing fence emissions to leading.  (Likely only killed on arm32)
  // see AtomicExpandPass.cpp:320, 1270, 1293
  TrailingFenceIsLeading,
  // One sub-mutation for each of the three insertions above.
  EndTrailingFenceIsLeading = TrailingFenceIsLeading + NumLeadingTrailingFences - 1,

  // Swaps the leading and trailing fences in position 0 of the above.
  // see AtomicExpandPass.cpp:318, 320
  SwapBracketFences,

  /*
   * AArch64ISelLowering
   */

  // Drop a special-case in AArch64 cmpxchg lowering that prevents LLSC lowering.
  // see AArch64ISelLowering.cpp:?
  AArch64ExpandCmpXchgO0ToLLSC,

  /*
   * ARMISelLowering
   */

  // Drop a special-case in ARM cmpxchg lowering that prevents LLSC lowering.
  // see ArmISellowering.cpp:?
  ARMExpandCmpXchgO0ToLLSC,

  // Drop leading and trailing DMB barrier emissions.
  // see ArmISelLowering.cpp:18180, 18183, 18184, 18204, 18205, 18206
  ARMDropDMB,
  // One sub-mutation for each case where we emit a DMB.
  EndARMDropDMB = ARMDropDMB + NumDMBs - 1,

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
