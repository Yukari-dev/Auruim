#include "auruim/registry.hpp"
#include "auruim/source.hpp"
#include <iostream>

using namespace Auruim;

int main() {
  Registry registry;

  std::string source = LoadFile("sandbox/main.aui").value();
  std::cout << source << '\n';

  return 0;
}
