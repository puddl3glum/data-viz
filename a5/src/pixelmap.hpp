#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <iostream>
#include <algorithm>

#include "vertex.hpp"
#include "line.hpp"

// using namespace std;

class Pixelmap {

  private:
    size_t width;
    size_t height;
    // double** pixels;
    std::vector<std::vector<double>> pixels;

    void mapsquare(std::vector<double>, std::vector<size_t>, double);
    // void interpolate(std::vector<Vertex>&, std::vector<Line>&);
    // void interpolate();
    double interpolate(double, double, double);

    size_t insertvertex(Vertex&);

    std::vector<Vertex> vertices;
    std::vector<Line> lines;

  public:
    Pixelmap();
    Pixelmap(uint8_t*, size_t, size_t);
    std::pair<std::vector<Vertex>, std::vector<Line>> march(double);
    std::pair<std::vector<Vertex>, std::vector<Line>> march(uint8_t);
    std::string to_string();
};
