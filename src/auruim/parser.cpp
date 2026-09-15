#include "auruim/parser.hpp"
#include "auruim/token.hpp"
#include "auruim/value.hpp"
#include <string>

namespace Auruim{

Parser::Parser(std::vector<Token> tokens) : m_tokens(std::move(tokens)) {}

Parser::Result Parser::Parse(){
  Node root = ParseNode();
  Expect(TokenType::EndOfFile, "end of file");
  return { std::move(root), std::move(m_errors) };
}

const Token& Parser::Peek(size_t offset) const{
  if(m_pos + offset >= m_tokens.size()) return m_tokens.back();
  return m_tokens[m_pos + offset];
}

const Token& Parser::Advance(){
  const Token& t = Peek();
  if(m_pos + 1 < m_tokens.size()) ++m_pos;
  return t;
}

bool Parser::Check(TokenType type) const {
  return Peek().tokenType == type;
}

bool Parser::Match(TokenType type){
  if(Check(type)) { Advance(); return true; }
  return false;
}

const Token& Parser::Expect(TokenType type, const std::string& what){
  if(!Check(type)){
    m_errors.push_back({
      "expected " + what + ", got " + std::string(TokenToString(Peek().tokenType)),
      Peek().line, Peek().column
    });
    return Peek();
  }
  return Advance();
}

Node Parser::ParseNode(){
  const Token& at = Expect(TokenType::AtSymbol, "@");

  Node node;
  node.line = at.line;
  node.column = at.column;

  node.type = Expect(TokenType::Identifier, "type name").value;
  node.id = Expect(TokenType::Identifier, "node id").value;

  Expect(TokenType::LeftCurlyBracket, "'{'");
  
  while(!Check(TokenType::RightCurlyBracket) && !Check(TokenType::EndOfFile)){
    size_t before = m_pos;
    if(Check(TokenType::AtSymbol)) node.children.push_back(ParseNode());
    else ParseProperty(node);

    if(m_pos == before) continue;
  }

  Expect(TokenType::RightCurlyBracket, "'}'");

  return node;
}

void Parser::ParseProperty(Node& node){
  const Token& propName = Expect(TokenType::Identifier, "Property name.");
  Expect(TokenType::Colon, "':'");
  Value v = ParseValue();
  auto [it, inserted] = node.props.emplace(propName.value, std::move(v));
  if(!inserted)
    m_errors.push_back({"duplicate property: " + propName.value, propName.line, propName.column});
}

Value Parser::ParseValue(){
  if(Check(TokenType::ColorString)){
    return Value::String(Advance().value);
  }

  if(Check(TokenType::Identifier))
    return Value::String(Advance().value);

  if(Check(TokenType::Number)){
    std::vector<Value> nums;
    float num = std::stof(Advance().value);
    nums.push_back(Value::Number(num));

    while(Match(TokenType::Comma))
      nums.push_back(Value::Number(std::stof(Expect(TokenType::Number, "number").value)));

    return nums.size() == 1 ? nums[0] : Value::Array(std::move(nums));
  }
  m_errors.push_back(
    {"expected value, got " + std::string(TokenToString(Peek().tokenType)), Peek().line, Peek().column}
  );
  return Value{};
}

}
