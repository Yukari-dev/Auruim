#include "auruim/load.hpp"
#include <iostream>

using namespace Auruim;

int main() {
  std::string path = "tests/test1.aui";

  auto result = LoadAndParse(path);

  auto& node = *result.node;
  std::cout << "type: " << node.type << '\n';
  std::cout << "id: " << node.id << '\n';
  for(auto& [k, v] : node.props)
    std::cout << "prop: " << k << "=" << v.ToDebugString() << "\n";

  for(auto& child : node.children){
    std::cout << "child Type: " << child.type << '\n';
    std::cout << "child id: " << child.id << '\n';
    for(auto& [k, v] : child.props)
      std::cout << "child prop: " << k << "=" << v.ToDebugString() << "\n";
  }

  return 0;
}
