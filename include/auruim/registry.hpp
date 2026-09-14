#pragma once
#include "token.hpp"
#include <optional>
#include <unordered_map>

namespace Auruim{

class Registry{
public:
  Registry();
  ~Registry() = default;

  std::optional<TokenType> Lookup(char c) const;
private:
  std::unordered_map<char, TokenType> m_symbols;
};

}
