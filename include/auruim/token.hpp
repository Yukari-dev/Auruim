#pragma once
#include <string>
#include <string_view>

namespace Auruim{

enum class TokenType{
  AtSymbol,
  LeftCurlyBracket,
  RightCurlyBracket,
  LeftParen,
  RightParen,
  Colon,
  Comma,
  Hash,

  Include,
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
    case TokenType::LeftParen: return "(";
    case TokenType::RightParen: return ")";
    case TokenType::Colon: return ":";
    case TokenType::Comma: return ",";
    case TokenType::Hash: return "#";
    case TokenType::Include: return "INCLUDE";
    case TokenType::Number: return "NUMBER";
    case TokenType::ColorString: return "COLOR STRING";
    case TokenType::StringLiteral: return "STRING LITERAL";
    case TokenType::Identifier: return "ID";
    case TokenType::Unknown: return "UNKNOWN";
    case TokenType::EndOfFile: return "EOF";
  }
  return "";
}

}
