#pragma once
#include "auruim/node.hpp"
#include "auruim/parser.hpp"
#include <optional>

namespace Auruim{

struct LoadResult{
  std::optional<Node> node;
  std::vector<Diagnostic> errors;
};

LoadResult LoadAndParse(const std::string& path);

}
