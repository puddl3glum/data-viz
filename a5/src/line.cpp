#include "line.hpp"

Line::Line(): start(0), end(0){}

Line::Line(size_t v0, size_t v1): start(v0), end(v1) {}

std::string Line::to_string() {
  return "l " + std::to_string(start) + " " + std::to_string(end);
}

Line& Line::operator=(const Line& other) {
   start = other.start;
   end = other.end;

   return *this;
}

bool Line::operator==(const Line& other) {

  return start == other.start && end == other.end;
}

bool Line::operator!=(const Line& other) {
  return ! (*this == other);
}
