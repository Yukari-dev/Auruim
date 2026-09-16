#include "auruim/resolver.hpp"
#include "auruim/load.hpp"
#include <filesystem>

namespace Auruim{


bool ResolveIncludes(Node& node, const std::string& baseDir, std::vector<Diagnostic>& errors){
  bool ok{true};

  for(auto& inc : node.includes){
    auto fullPath = (std::filesystem::path(baseDir) / inc.path).string();

    auto result = ParseFile(fullPath);
    if(!result.node){
      for(auto& d : result.errors) errors.push_back(d);
      ok = false;
      continue;
    }

    auto nestedDir = std::filesystem::path(fullPath).parent_path().string();
    if (!ResolveIncludes(*result.node, nestedDir, errors)) ok = false;
    node.children.push_back(std::move(*result.node));
  }
  
  for(auto& child : node.children){
    if(!ResolveIncludes(child, baseDir, errors)) ok = false;
  }
    
  return ok;
}

}
