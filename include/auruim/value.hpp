#pragma once

#include <cassert>
#include <string>
#include <variant>
#include <vector>
namespace Auruim{

class Value{
public:
  static Value Number(float v);
  static Value Bool(bool b);
  static Value String(std::string s);
  static Value Array(std::vector<Value> a);

  bool IsArray() const{
    return std::holds_alternative<std::vector<Value>>(m_data);
  }

  const std::vector<Value>& AsArray() const {
    auto* p = std::get_if<std::vector<Value>>(&m_data);
    assert(p && "Value is not an Array");
    return *p;
  }

private:
  std::variant<std::monostate, float, bool, std::string, std::vector<Value>> m_data;
};

}
