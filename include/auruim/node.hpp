#pragma once
#include "auruim/value.hpp"
#include <string>
#include <unordered_map>
#include <vector>

namespace Auruim{

struct Node{
  std::string type{};
  std::string id{};
  std::unordered_map<std::string, Value> props{};
  std::vector<Node> children{};
  size_t line{1}, column{1};
};

}
