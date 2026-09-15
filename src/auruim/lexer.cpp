#include "auruim/lexer.hpp"

namespace Auruim{

Lexer::Lexer(std::string_view source, const Registry& symb) : m_source(source), m_symbols(symb) {}

std::vector<Token> Lexer::Tokenize(){
  std::vector<Token> tokens;

  while(!AtEnd()){
    char c = Peek();
    
    if(std::isspace(static_cast<unsigned char>(c))){
      SkipWhitespace();
      continue;
    }

    size_t startPos{m_pos}, startLine{m_line}, startCol{m_col};
    if(c == '#') { EmitColor(tokens); continue; }
    if(auto t = m_symbols.Lookup(c)){
      Advance();
      tokens.push_back(MakeToken(*t, startPos, startLine, startCol));
      continue;
    }
    if(IsDigit(c) || c == '.') { EmitNumber(tokens); continue; }
    if(std::isalpha(static_cast<unsigned char>(c))) { EmitIdentifier(tokens); continue; }

    Advance();
  }
  tokens.push_back(MakeToken(TokenType::EndOfFile, m_pos, m_line, m_col));
  return tokens;
}

char Lexer::Peek(size_t offset) const{
  if(m_pos + offset >= m_source.size()) return '\0';
  return m_source[m_pos + offset];
}

char Lexer::Advance(){
  if(m_pos >= m_source.size()) return '\0';
  char c = m_source[m_pos++];
  if(c == '\n') { ++m_line; m_col = 1; }
  else m_col++;
  return c;
}

bool Lexer::AtEnd() const{
  return m_pos >= m_source.size();
}

void Lexer::SkipWhitespace(){
  while(!AtEnd() && std::isspace(static_cast<unsigned char>(Peek())))
    Advance();
}

void Lexer::EmitNumber(std::vector<Token>& out){
  size_t startPos = m_pos, startLine = m_line, startCol = m_col;
  while(!AtEnd() && IsDigit(Peek())) Advance();
  if(!AtEnd() && Peek() == '.' && IsDigit(Peek(1))){
    Advance();
    while(!AtEnd() && IsDigit(Peek())) Advance();
  }
  out.push_back(MakeToken(TokenType::Number, startPos, startLine, startCol));
}

void Lexer::EmitIdentifier(std::vector<Token>& out){
  size_t startPos = m_pos, startLine = m_line, startCol = m_col;
  while(!AtEnd() && std::isalnum(static_cast<unsigned char>(Peek()))) Advance();
  out.push_back(MakeToken(TokenType::Identifier, startPos, startLine, startCol));
}

void Lexer::EmitColor(std::vector<Token>& out){
  size_t startPos = m_pos, startLine = m_line, startCol = m_col;
  Advance();
  while(!AtEnd() && IsHex(Peek())) Advance();
  out.push_back(MakeToken(TokenType::ColorString, startPos, startLine, startCol));
}

bool Lexer::IsDigit(char c) const{
  return std::isdigit(static_cast<unsigned char>(c));
}

bool Lexer::IsHex(char c) const{
  return IsDigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

Token Lexer::MakeToken(TokenType type, size_t startPos, size_t startLine, size_t startCol){
  return Token{
    .tokenType = type,
    .value = std::string(m_source.substr(startPos, m_pos - startPos)),
    .line = startLine,
    .column = startCol
  };
}

}
