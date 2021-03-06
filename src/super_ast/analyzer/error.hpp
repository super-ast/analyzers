#pragma once

#include <iostream>

class Error {
public:
  int line_number;
  std::string function;
  std::string short_desc;
  std::string long_desc;

  Error(int ln, std::string f, std::string s, std::string l) :
      line_number(ln), function(f), short_desc(s), long_desc(l) {
  }
};
