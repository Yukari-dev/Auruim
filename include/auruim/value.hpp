#pragma once

#include <cassert>
#include <string>
#include <variant>
#include <vector>
namespace Auruim{

class Value{
public:
  Value() = default;
  static Value Number(float v) { return Value{v}; }
  static Value Bool(bool b) { return Value{b}; }
  static Value String(std::string s) { return Value{s}; }
  static Value Array(std::vector<Value> a) { return Value{a}; }

  bool IsArray() const{
    return std::holds_alternative<std::vector<Value>>(m_data);
  }

  template <typename T>
  const T& AsData() const{
    auto* p = std::get_if<T>(&m_data);
    assert(p && "Value is not what it expect");
    return *p;
  }
  template <typename T>
  T& AsData() {
    auto* p = std::get_if<T>(&m_data);
    assert(p && "Value is not what it expects");
    return *p;
  }

  const std::vector<Value>& AsArray() const {
    auto* p = std::get_if<std::vector<Value>>(&m_data);
    assert(p && "Value is not an Array");
    return *p;
  }

  std::string ToDebugString() const {
    if (auto p = std::get_if<float>(&m_data)) return std::to_string(*p);
    if (auto p = std::get_if<std::string>(&m_data)) return '"' + *p + '"';
    if (auto p = std::get_if<std::vector<Value>>(&m_data)) {
      std::string s = "[";
      for (size_t i = 0; i < p->size(); ++i) {
        if (i) s += ", ";
        s += (*p)[i].ToDebugString();
      }
      return s + "]";
    }
    if (std::holds_alternative<bool>(m_data))
      return std::get<bool>(m_data) ? "true" : "false";
    return "none";
  }

  bool operator==(const Value& other) const {
    return m_data == other.m_data;
  }
private:
  Value(float v) : m_data(v) {}
  Value(bool v) : m_data(v) {}
  Value(std::string v) : m_data(v) {}
  Value(std::vector<Value> v) : m_data(v) {}
  std::variant<std::monostate, float, bool, std::string, std::vector<Value>> m_data;
};

}
