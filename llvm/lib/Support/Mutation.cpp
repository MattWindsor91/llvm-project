//
// Created by Matthew Windsor on 15/01/2021.
//

#include "llvm/Support/Mutation.h"
#include <cstdlib>
#include <iostream>
#include <string>

namespace llvm {
llvm::Mutation C4Mutation = llvm::Mutation::None;


const std::string_view mutationName(Mutation m){
  if (Mutation::FlipIsStrongerThan <= m && m < Mutation::EndFlipIsStrongerThan) {
    return "flip isStrongerThan";
  }
  if (Mutation::FlipIsAtLeastOrStrongerThan <= m && m < Mutation::EndFlipIsAtLeastOrStrongerThan) {
    return "flip isAtLeastOrStrongerThan";
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

void SetupMutation() {
  auto lstr = getenv("C4_MUTATION");
  if (lstr == nullptr) return;

  std::string lsstr{lstr};
  auto lint = std::stoi(lsstr);
  C4Mutation = static_cast<llvm::Mutation>(lint % static_cast<int>(llvm::Mutation::Count));

  std::cerr << "MUTATION SELECTED: " << lint << "(= " << mutationName(C4Mutation) << ")" << std::endl;
}
}
