#include "auruim/source.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

namespace Auruim{

std::optional<std::string> LoadFile(const std::string& path){
  if(!path.ends_with(".aui")){
    std::cerr << "The file must have '.aui' extension.\n";
    return std::nullopt;
  }
  std::ifstream file(path, std::ios::binary);
  if(!file) return std::nullopt;

  std::ostringstream ss;
  ss << file.rdbuf();
  if(file.bad()) return std::nullopt;

  std::string content = ss.str();
  if(content.size() >= 3 &&
    (unsigned char)content[0] == 0xEF &&
    (unsigned char)content[1] == 0xBB &&
    (unsigned char)content[2] == 0xBF
  )
    content.erase(0, 3);

  return content;
}

}
