
#include <string>

#include "vertex.hpp"


Vertex::Vertex(): x(0), y(0) {}

Vertex::Vertex(double _x, double _y): x(_x), y(_y) {}

Vertex& Vertex::operator= (const Vertex& other) {
  x = other.x;
  y = other.y;

  return *this;
}

Vertex Vertex::operator+(const Vertex& other) {
  return Vertex(x + other.x, y + other.y);
}

Vertex Vertex::operator/(const int i) {
  return Vertex(x / i, y / i);
}

bool Vertex::operator== (const Vertex& other) {
  return x == other.x && y == other.y;
}

std::string Vertex::to_string() {

  return "v " + std::to_string(x) + " " + std::to_string(y) + " 0";
  
}
