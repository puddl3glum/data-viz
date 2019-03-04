#pragma once

#include <cstddef>
#include <string>

class Line {

  private:

  public:
    size_t start;
    size_t end;
    Line();
    Line(size_t, size_t);
    std::string to_string();
    Line& operator=(const Line&);
    bool operator==(const Line&);
    bool operator!=(const Line&);
};
