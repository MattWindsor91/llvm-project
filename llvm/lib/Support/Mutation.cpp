//
// Created by Matthew Windsor on 15/01/2021.
//

#include "llvm/Support/Mutation.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <map>

namespace llvm {
// Global variable tracking the current mutation.
llvm::Mutation C4Mutation = llvm::Mutation::None;


const char* mutationName(Mutation m){
  // This should be in increasing order of mutation ID, to let the linear search work.
  std::map<Mutation, const char*> names = {
      {Mutation::None, "none"},
      {Mutation::DropVolatileGuard, "DVG"},
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
  // Because the mutant might have a variant offset, we need to find the *last*
  // mutant lesser than or equal to m.
  for (auto It = names.crbegin(); It != names.crend(); ++It) {
    if (It->first <= m) {
      return It->second;
    }
  }
  return "unknown";
}

void setupMutation() {
  auto *Cstr = getenv("C4_MUTATION");
  if (Cstr == nullptr) return;

  std::string Str{Cstr};
  auto Int = std::stoi(Str);
  auto IntMod = Int % static_cast<int>(llvm::Mutation::Count);
  C4Mutation = static_cast<llvm::Mutation>(IntMod);

  std::cerr << "MUTATION SELECTED: " << IntMod << " (= " << mutationName(C4Mutation) << ", input=" << Int << ")" << std::endl;
}
}
