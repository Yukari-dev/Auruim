#include "auruim/lexer.hpp"
#include "auruim/parser.hpp"
#include "auruim/registry.hpp"
#include "auruim/source.hpp"
#include <iostream>

using namespace Auruim;

int main() {
  std::string path = "tests/test1.aui";
  Registry registry;
  std::string source = LoadFile(path).value();
  Lexer lexer(source, registry);
  Parser parser(lexer.Tokenize());
  auto result = parser.Parse();
  if(!result.errors.empty()){
    for(auto& d : result.errors)
      std::cerr << path << ":" << d.line << ":" << d.column << ": " << d.message << '\n';
    return 1;
  }

  auto& node = *result.node;
  std::cout << "type: " << node.type << '\n';
  std::cout << "id: " << node.id << '\n';
  for(auto& [k, v] : node.props){
    std::cout << "prop: " << k << "=" << v.ToDebugString() << "\n";
  }

  return 0;
}
