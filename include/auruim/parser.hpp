#pragma once
#include "auruim/node.hpp"
#include "auruim/token.hpp"
#include "auruim/diagnostic.hpp"
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace Auruim{

struct ParseError : std::runtime_error{
  size_t line{}, column{};
  ParseError(std::string msg, size_t l, size_t c) : std::runtime_error(std::move(msg)), line(l), column(c) {}
};

class Parser{
public:
  explicit Parser(std::vector<Token> tokens);
  
  struct Result{
    std::optional<Node> node;
    std::vector<Include> includes;
    std::vector<Diagnostic> errors;
  };

  Result Parse();

private:
  const Token& Peek(size_t offset = 0) const;
  const Token& Advance();
  bool Check(TokenType type) const;
  bool Match(TokenType type);
  const Token& Expect(TokenType type, const std::string& what);

  Node ParseNode();
  void ParseProperty(Node& node);
  void ParseInclude(Node& node);
  Value ParseValue();
  Value ParseFunctionCall();
private:
  std::vector<Token> m_tokens{};
  size_t m_pos{};
  std::vector<Diagnostic> m_errors{};
};

}
