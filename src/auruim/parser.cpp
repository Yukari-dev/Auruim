#include "auruim/parser.hpp"
#include "auruim/token.hpp"
#include "auruim/value.hpp"
#include <algorithm>
#include <cstdio>
#include <string>

namespace Auruim{

Parser::Parser(std::vector<Token> tokens) : m_tokens(std::move(tokens)) {}

Parser::Result Parser::Parse(){
  std::vector<Include> includes{};
  while(Check(TokenType::Include)){
    Node dummy{};
    ParseInclude(dummy);
    for(auto& inc : dummy.includes) includes.push_back(std::move(inc));
  }
  Node root = ParseNode();
  Expect(TokenType::EndOfFile, "end of file");
  return { std::move(root), std::move(includes), std::move(m_errors) };
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

  if(Check(TokenType::Identifier)) node.id = Advance().value;
  else node.id = "node_" + std::to_string(node.line) + "_" + std::to_string(node.column);

  Expect(TokenType::LeftCurlyBracket, "'{'");
  
  while(!Check(TokenType::RightCurlyBracket) && !Check(TokenType::EndOfFile)){
    size_t before = m_pos;

    if(Check(TokenType::Include)) ParseInclude(node);
    else if(Check(TokenType::AtSymbol)) node.children.push_back(ParseNode());
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
  auto it = std::find_if(node.props.begin(), node.props.end(), [&](const auto& p){ return p.first == propName.value; });
  if(it != node.props.end())
    m_errors.push_back({"duplicate property: " + propName.value, propName.line, propName.column});
  else
    node.props.emplace_back(propName.value, std::move(v));
}

Value Parser::ParseValue(){
  if(Check(TokenType::ColorString) || Check(TokenType::StringLiteral))
    return Value::String(Advance().value);

  if(Check(TokenType::Identifier) && Peek(1).tokenType == TokenType::LeftParen)
    return ParseFunctionCall();

  if(Check(TokenType::Identifier))
    return Value::String(Advance().value);

  if(Check(TokenType::Number)){
    std::vector<Value> nums;
    float num = std::stof(Advance().value);
    nums.push_back(Value::Number(num));

    while(Match(TokenType::Comma)){
      if(!Check(TokenType::Number)) break;
      nums.push_back(Value::Number(std::stof(Expect(TokenType::Number, "number").value)));
    }

    return nums.size() == 1 ? nums[0] : Value::Array(std::move(nums));
  }
  m_errors.push_back(
    {"expected value, got " + std::string(TokenToString(Peek().tokenType)), Peek().line, Peek().column}
  );
  return Value{};
}

Value Parser::ParseFunctionCall(){
  const Token& name = Advance();

  Expect(TokenType::LeftParen, "'('");

  std::vector<float> args;
  args.push_back(std::stof(Expect(TokenType::Number, "number").value));

  while(Match(TokenType::Comma)){
    if(!Check(TokenType::Number)){
      m_errors.push_back({"expected number after ','", Peek().line, Peek().column});
      break;
    }
    args.push_back(std::stof(Advance().value));
  }

  Expect(TokenType::RightParen, "')'");
  
  if(name.value == "rgb"){
    if(args.size() != 3){
      m_errors.push_back({"rgb() requires 3 arguments", name.line, name.column});
      return Value::String("#000000");
    }
    int r = std::clamp((int)args[0], 0, 255);
    int g = std::clamp((int)args[1], 0, 255);
    int b = std::clamp((int)args[2], 0, 255);
    char buf[8];
    std::snprintf(buf, sizeof(buf), "#%02X%02X%02X", r, g, b);
    return Value::String(buf);
  }

  m_errors.push_back({"unknown function: " + name.value, name.line, name.column});
  return Value::String("");
}

void Parser::ParseInclude(Node& node){
  const Token& kw = Advance();
  
  if(!Check(TokenType::StringLiteral)){
    m_errors.push_back({"expected file path after 'include'", Peek().line, Peek().column});
    return;
  }

  const Token& pathTok = Advance();

  Include inc{
    .path = pathTok.value,
    .line = pathTok.line,
    .column = pathTok.column
  };
  node.includes.push_back(std::move(inc));
}

}
