#include "auruim/lexer.hpp"
#include "auruim/registry.hpp"
#include "auruim/source.hpp"

using namespace Auruim;

int main() {
  Registry registry;
  std::string source = LoadFile("sandbox/main.aui").value();
  Lexer lexer(source, registry);
  lexer.Tokenize();

  return 0;
}
