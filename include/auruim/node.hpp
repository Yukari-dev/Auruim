#pragma once
#include "auruim/value.hpp"
#include <string>
#include <utility>
#include <vector>

namespace Auruim{

struct Include{
  std::string path{};
  size_t line{}, column{};
};

struct Node{
  std::string type{};
  std::string id{};
  std::vector<std::pair<std::string, Value>> props;
  std::vector<Node> children{};
  std::vector<Include> includes{};
  size_t line{1}, column{1};
};

}
