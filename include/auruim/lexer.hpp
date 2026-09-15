#pragma once
#include "auruim/registry.hpp"
#include "auruim/token.hpp"
#include <string_view>
#include <vector>

namespace Auruim{

class Lexer{
public:
  explicit Lexer(std::string_view source, const Registry& symbols);

  std::vector<Token> Tokenize();
private:
  char Peek(size_t offset = 0) const;
  char Advance();
  bool AtEnd() const;

  void SkipWhitespace();
  void EmitNumber(std::vector<Token>& out);
  void EmitIdentifier(std::vector<Token>& out);
  void EmitString(std::vector<Token>& out);
  void EmitColor(std::vector<Token>& out);

  bool IsDigit(char c) const;
  bool IsHex(char c) const;

  Token MakeToken(TokenType type, size_t startPos, size_t startLine, size_t startCol);
private:
  std::string_view m_source;
  const Registry& m_symbols;
  size_t m_pos{0};
  size_t m_line{1};
  size_t m_col{1};
};

}
