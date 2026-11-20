#pragma once
#include <string>

struct Word {
  int id = 0;
  std::string english;
  std::string arabic;
  std::string source;
  std::string dialect;
  std::string wordform;
  std::string category;
  std::string tags;
  bool exported = false;
};
