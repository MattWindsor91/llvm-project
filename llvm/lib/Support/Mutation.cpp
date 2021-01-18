//
// Created by Matthew Windsor on 15/01/2021.
//

#include "llvm/Support/Mutation.h"
#include <cstdlib>
#include <iostream>
#include <string>

namespace llvm {
llvm::Mutation C4Mutation = llvm::Mutation::None;

void SetupMutation() {
  auto lstr = getenv("C4_MUTATION");
  if (lstr == nullptr) return;

  std::string lsstr{lstr};
  auto lint = std::stoi(lsstr);
  C4Mutation = static_cast<llvm::Mutation>(lint % static_cast<int>(llvm::Mutation::Count));

  std::cerr << "MUTATION SELECTED: " << lint << std::endl;
}
}
