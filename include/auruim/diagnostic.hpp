#pragma once
#include <string>

namespace Auruim{

struct Diagnostic{
  std::string message{};
  size_t line{}, column{};
};

}
