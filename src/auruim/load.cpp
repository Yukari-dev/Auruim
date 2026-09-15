#include "auruim/load.hpp"
#include "auruim/lexer.hpp"
#include "auruim/registry.hpp"
#include "auruim/source.hpp"
#include <iostream>

namespace Auruim{

LoadResult LoadAndParse(const std::string& path) {
  auto source = LoadFile(path);
  if (!source)
    return { std::nullopt, {{"cannot read file: " + path, 0, 0}} };

  Registry symbols;
  Lexer lexer(*source, symbols);
  auto tokens = lexer.Tokenize();

  Parser parser(std::move(tokens));
  auto result = parser.Parse();

  return { std::move(result.node), std::move(result.errors) };
}

}
