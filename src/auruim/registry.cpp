#include "auruim/registry.hpp"
#include "auruim/token.hpp"

namespace Auruim {

Registry::Registry(){
  m_symbols['@'] = TokenType::AtSymbol;
  m_symbols['{'] = TokenType::LeftCurlyBracket;
  m_symbols['}'] = TokenType::RightCurlyBracket;
  m_symbols[':'] = TokenType::Colon;
  m_symbols[','] = TokenType::Comma;
  m_symbols['#'] = TokenType::Hash;
  m_symbols['('] = TokenType::LeftParen;
  m_symbols[')'] = TokenType::RightParen;
}

std::optional<TokenType> Registry::Lookup(char c) const{
  auto it = m_symbols.find(c);
  if(it == m_symbols.end()) return std::nullopt;
  return it->second;
}

}
