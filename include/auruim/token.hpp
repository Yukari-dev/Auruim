#pragma once
#include <string>
#include <string_view>

namespace Auruim{

enum class TokenType{
  AtSymbol,
  LeftCurlyBracket,
  RightCurlyBracket,
  Colon,
  Comma,
  Hash,

  Number,
  ColorString,
  StringLiteral,
  Identifier,
  Unknown,
  EndOfFile
};

struct Token{
  TokenType tokenType{};
  std::string value{};
  size_t line{1}, column{1};
};

constexpr std::string_view TokenToString(TokenType t) noexcept{
  switch(t){
    case TokenType::AtSymbol: return "@";
    case TokenType::LeftCurlyBracket: return "{";
    case TokenType::RightCurlyBracket: return "}";
    case TokenType::Colon: return ":";
    case TokenType::Comma: return ",";
    case TokenType::Hash: return "#";
    case TokenType::Number: return "NUMBER";
    case TokenType::Identifier: return "ID";
    case TokenType::EndOfFile: return "EOF";
  }
  return "";
}

}
