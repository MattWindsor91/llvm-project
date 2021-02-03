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

// Number of variants for MarkRMWIdempotent*: add, sub, or, xor, and, xchg.
// These are also the number of variants for MarkRMWSaturatingCombine, less one
// because xchg is already true.
constexpr uint16_t NumRMWCases = 6;

// Number of leading/trailing fence pairs: 0 in bracket, 1 and 2 in cmpxchg.
// 0 is important as it is the target of the SwapBracketFences mutation.
constexpr uint16_t NumLeadingTrailingFences = 3;

// Number of DMB making endpoints in ARMISelLowering.
constexpr uint16_t NumDMBs = 6;

// Number of sync making endpoints in PPCISelLowering.
constexpr uint16_t NumSyncs = 4;


enum class Mutation : std::uint16_t {
  None = 0,

  /*
   * AtomicOrdering
   */

  // [FIS] Flips bits in the isStrongerThan table.
  //
  // Rationale: simulate typos in the table.
  //
  // see AtomicOrdering.h:102
  FlipIsStrongerThan,
  // One variant for each pair of (viable) memory orders.
  EndFlipIsStrongerThan = FlipIsStrongerThan + MemOrderEntries - 1,

  // [FIA] As above but for IsAtLeastOrStrongerThan.
  //
  // Rationale: simulate typos in the table.
  //
  // see AtomicOrdering.h:117
  FlipIsAtLeastOrStrongerThan,
  // One variant for each pair of (viable) memory orders.
  EndFlipIsAtLeastOrStrongerThan = FlipIsAtLeastOrStrongerThan + MemOrderEntries - 1,

  // [WCA] Set acq/rel/acqrel/seq_cst (in that order) to relaxed in the C ABI mapping.
  // see AtomicOrdering.h:147
  WeakenCABI,
  // One sub-mutation for each memory order above.
  EndWeakenCABI = WeakenCABI + NumCABIWeakenings - 1,

  /*
   * AtomicExpandPass
   */

  // [RIE] Spuriously mark a RMW as idempotent. (Likely only killed on x86)
  // see AtomicExpandPass.cpp:1360, 1361, 1362, 1364, 1366.
  MarkRMWIdempotentExpand,
  // One variant for each case in the switch statement.
  EndMarkRMWIdempotentExpand = MarkRMWIdempotentExpand + NumRMWCases - 1,

  // [LFT] Change leading fence emissions to trailing.
  //
  // Rationale: bracketing fence emission swap is a known compiler bug.
  //
  // see AtomicExpandPass.cpp:318, 1210, 1232
  LeadingFenceIsTrailing,
  // One variant for each of the three insertions above.
  EndLeadingFenceIsTrailing = LeadingFenceIsTrailing + NumLeadingTrailingFences - 1,

  // [TFL] Change trailing fence emissions to leading.
  //
  // Rationale: bracketing fence emission swap is a known compiler bug.
  //
  // see AtomicExpandPass.cpp:320, 1270, 1293
  TrailingFenceIsLeading,
  // One variant for each of the three insertions above.
  EndTrailingFenceIsLeading = TrailingFenceIsLeading + NumLeadingTrailingFences - 1,

  // [SLT] Swaps the leading and trailing fences in position 0 of the above.
  //
  // Rationale: bracketing fence emission swap is a known compiler bug.
  //
  // see AtomicExpandPass.cpp:318, 320
  SwapBracketFences,

  /*
   * InstCombineAtomicRMW
   */

  // [RIC] As MarkRMWIdempotentExpand, but as part of the InstCombineAtomicRMW group.
  // see InstCombineAtomicRMW.cpp:39, 40, 41, 43, 45, 55.
  MarkRMWIdempotentCombine,
  // One variant for each case in the switch statement.
  EndMarkRMWIdempotentCombine = MarkRMWIdempotentCombine + NumRMWCases - 1,

  // [RSC] Spuriously marks RMWs as saturating in InstCombineAtomicRMW.
  // see InstCombineAtomicRMW.cpp:77, 77 (not a typo), 81, 77 (also not a typo), 83.
  MarkRMWSaturatingCombine,
  // One variant for each case in the switch statement, except xchg.
  EndMarkRMWSaturatingCombine = MarkRMWSaturatingCombine + (NumRMWCases - 1) - 1,

  // Disables isVolatile() gate on visitAtomicRMWInst.
  //
  // Rationale: comment ('we chose not to canonicalize out of general paranoia
  // about user expectations around volatile).
  //
  // see InstCombineAtomicRMW.cpp:101
  VisitVolatileRMW,

  /*
   * AArch64ISelLowering
   */

  // [DXG] Drop a special-case in AArch64 cmpxchg lowering that prevents LLSC lowering.
  //
  // Rationale: comment above suggests this sidesteps a failure condition.
  //
  // see AArch64ISelLowering.cpp:14626
  AArch64ExpandCmpXchgO0ToLLSC,

  /*
   * ARMISelLowering
   */

  // [DXG] Drop a special-case in ARM cmpxchg lowering that prevents LLSC lowering.
  //
  // Rationale: comment above suggests this sidesteps a failure condition.
  //
  // see ARMISelLowering.cpp:18260
  ARMExpandCmpXchgO0ToLLSC,

  // [DDF] Drop leading and trailing DMB barrier emissions.
  //
  // Rationale: fence omission is a classic compiler bug.
  //
  // see ARMISelLowering.cpp:18180, 18183, 18184, 18204, 18205, 18206
  ARMDropDMB,
  // One variant for each case where we emit a DMB.
  EndARMDropDMB = ARMDropDMB + NumDMBs - 1,

  /*
   * PPCISelLowering
   */

  // [DSF] Drop sync barrier emissions.
  //
  // Rationale: fence omission is a classic compiler bug.
  //
  // see PPCISelLowering.cpp:11303, 11305, 11317, 11323
  PPCDropSync,
  // One variant for each case where we emit a sync.
  EndPPCDropSync = PPCDropSync + NumSyncs - 1,

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
const char *mutationName(Mutation M);
}

#endif // LLVM_MUTATION_H
