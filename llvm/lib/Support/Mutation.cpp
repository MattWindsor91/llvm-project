//
// Created by Matthew Windsor on 15/01/2021.
//

#include "llvm/Support/Mutation.h"
#include <cstdlib>
#include <iostream>
#include <string>

namespace llvm {
// Global variable tracking the current mutation.
llvm::Mutation C4Mutation = llvm::Mutation::None;


const std::string_view mutationName(Mutation m){
  if (Mutation::FlipIsStrongerThan <= m && m <= Mutation::EndFlipIsStrongerThan) {
    return "flip isStrongerThan";
  }
  if (Mutation::FlipIsAtLeastOrStrongerThan <= m && m <= Mutation::EndFlipIsAtLeastOrStrongerThan) {
    return "flip isAtLeastOrStrongerThan";
  }
  if (Mutation::WeakenCABI <= m && m <= Mutation::WeakenCABI) {
    return "weaken CABI"
  }

  switch (m) {
  case Mutation::None:
    return "none";
  case Mutation::AArch64ExpandCmpXchgO0ToLLSC:
    return "AArch64 cmpxchg -O0 -> LLSC";
  case Mutation::ARMExpandCmpXchgO0ToLLSC:
    return "ARM cmpxchg -O0 -> LLSC";
  default:
    break;
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
