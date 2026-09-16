#include "auruim/load.hpp"
#include "auruim/lexer.hpp"
#include "auruim/parser.hpp"
#include "auruim/registry.hpp"
#include "auruim/resolver.hpp"
#include "auruim/source.hpp"
#include <filesystem>

namespace Auruim{

LoadResult ParseFile(const std::string& path){
  auto source = LoadFile(path);
  if (!source)
    return { std::nullopt, {{"cannot read file: " + path, 0, 0}} };

  Registry symbols;
  Lexer lexer(*source, symbols);
  auto tokens = lexer.Tokenize();

  Parser parser(std::move(tokens));
  auto result = parser.Parse();

  return { std::move(result.node), std::move(result.includes), std::move(result.errors) };
}

LoadResult LoadAndParse(const std::string& path) {
  auto parsed = ParseFile(path);
  if (!parsed.node)
    return parsed;

  Node root = std::move(*parsed.node);
  auto baseDir = std::filesystem::path(path).parent_path().string();

  for(auto& inc : parsed.includes)
    root.includes.push_back(std::move(inc));
  ResolveIncludes(root, baseDir, parsed.errors);

  return { std::move(root), std::move(parsed.includes), std::move(parsed.errors) };
}

}
