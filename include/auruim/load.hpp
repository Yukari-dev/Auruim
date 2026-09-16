#pragma once
#include "auruim/node.hpp"
#include "auruim/diagnostic.hpp"
#include <optional>

namespace Auruim{

struct LoadResult{
  std::optional<Node> node;
  std::vector<Include> includes;
  std::vector<Diagnostic> errors;
};

LoadResult ParseFile(const std::string& path);

LoadResult LoadAndParse(const std::string& path);

}
