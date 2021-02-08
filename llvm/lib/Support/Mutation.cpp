//
// Created by Matthew Windsor on 15/01/2021.
//

#include "llvm/Support/Mutation.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <map>

namespace llvm {
// Global variable tracking the current mutation.
llvm::Mutation C4Mutation = llvm::Mutation::None;

// This should be in increasing order of mutation ID, to let the linear search work.
const std::map<Mutation, const char*> MutationNames = {
    {Mutation::None, "none"},
    {Mutation::DropAtomicGuard, "DAG"},
    {Mutation::DropVolatileGuard, "DVG"},
    {Mutation::DropUnorderedGuard, "DUG"},
    {Mutation::FlipIsStrongerThan, "FIS"},
    {Mutation::FlipIsAtLeastOrStrongerThan, "FIA"},
    {Mutation::WeakenCABI, "WCA"},
    {Mutation::MarkRMWIdempotentExpand, "RIE"},
    {Mutation::LeadingFenceIsTrailing, "LFT"},
    {Mutation::TrailingFenceIsLeading, "TFL"},
    {Mutation::SwapBracketFences, "SLT"},
    {Mutation::MarkRMWIdempotentCombine, "RIC"},
    {Mutation::MarkRMWSaturatingCombine, "RSC"},
    {Mutation::AArch64ExpandCmpXchgO0ToLLSC, "DXG[aarch64]"},
    {Mutation::ARMExpandCmpXchgO0ToLLSC, "DXG[arm]"},
    {Mutation::ARMDropAtomicGuard, "DAG[arm]"},
    {Mutation::ARMDropDMB, "DDF[arm]"},
    {Mutation::PPCDropSync, "DSF[ppc]"},
};

Mutation parentMutation(Mutation M) {
  // Because the mutant might have a variant offset, we need to find the *last*
  // mutant lesser than or equal to m.
  return find_if(MutationNames.crbegin(), MutationNames.crend(), [=](auto M2) { return M2.first <= M; })->first;
}

const char* mutationName(Mutation M){
  auto It = MutationNames.find(M);
  if (It == MutationNames.end()) {
    return "unknown";
  }
  return It->second;
}

void setupMutation() {
  auto *Cstr = getenv("C4_MUTATION");
  if (Cstr == nullptr) return;

  std::string Str{Cstr};
  auto Int = std::stoi(Str);
  auto IntMod = Int % static_cast<int>(llvm::Mutation::Count);
  C4Mutation = static_cast<llvm::Mutation>(IntMod);
  auto Parent = parentMutation(C4Mutation);
  auto Variant = IntMod - static_cast<int16_t>(Parent);

  std::cerr << "MUTATION SELECTED: " << IntMod << " (= " << mutationName(Parent) << ":" << Variant << ", input=" << Int << ")" << std::endl;
}

bool c4MutationEnabled() {
  return C4Mutation != Mutation::None;
}
}
