#pragma once
#include "auruim/node.hpp"

namespace Auruim{

struct Diagnostic;

bool ResolveIncludes(Node& node, const std::string& baseDir, std::vector<Diagnostic>& errors);

}
