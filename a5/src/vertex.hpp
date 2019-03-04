#pragma once

#include <string>

class Vertex {
  private:
  public:
    double x;
    double y;
    Vertex();
    Vertex(double, double);
    Vertex& operator=(const Vertex&);
    Vertex operator+(const Vertex&);
    Vertex operator/(const int);
    Vertex& operator/(const Vertex&);
    bool operator==(const Vertex&);
    std::string to_string();
};

