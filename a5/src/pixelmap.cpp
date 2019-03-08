
// #include <cstddef>

#include "pixelmap.hpp"

#include "vertex.hpp"
#include "line.hpp"

Pixelmap::Pixelmap(){}

Pixelmap::Pixelmap(uint8_t* data, size_t w, size_t h) {

  
  /*
  for (size_t row = 0; row < height; row++) {
    for (size_t column = 0; column < width; column++ ) {
      // printf("%d ", data[row * height + column]);
      uint8_t brightness = data[row * height + column];
      data[row * height + column] = (brightness > isovalue) ? 1 : 0;
        // SDL_SetRenderDrawColor(renderer, brightness, brightness, brightness, 255);
    }
  }
  */
  
  width = w;
  height = h;

  for (size_t x = 0; x < height; x++) {
    std::vector<double> row;
    for (size_t y = 0; y < width; y++) {
      double pixel = (double) data[x * height + y] / 255;
      // cout << pixel << endl;
      row.push_back(pixel);
    }

    pixels.push_back(row);

  }
}

size_t Pixelmap::insertvertex(Vertex& v) {
  size_t index;

  auto it = std::find(vertices.begin(), vertices.end(), v);

  if (it != vertices.end()) {
    index = std::distance(vertices.begin(), it) + 1;
  }
  else {
    vertices.push_back(v);
    index = vertices.size();
  }

  return index;

}

double Pixelmap::interpolate(double weight0, double weight1, double isovalue) {
  return (isovalue - weight0) / (weight1 - weight0);
}


void Pixelmap::mapsquare(std::vector<double> brightness, std::vector<size_t> position, double isovalue) {

  // bitmap whether points are above or below isovalue
  // fit line
  
  // upper left
  double ul = brightness[0];
  // upper right
  double ur = brightness[1];
  // lower left
  double ll = brightness[2];
  // lower right
  double lr = brightness[3];

  double lcol = position[0];
  double rcol = position[1];
  double urow = position[2];
  double lrow = position[3];

  double lcolmid = interpolate(ul, ll, isovalue);
  double rcolmid = interpolate(ur, lr, isovalue);
  double urowmid = interpolate(ur, ul, isovalue);
  double lrowmid = interpolate(lr, ll, isovalue);
  
  uint8_t bitmap = 0b000;

  bitmap += ul >= isovalue;
  bitmap <<= 1;
  bitmap += ur >= isovalue;
  bitmap <<= 1;
  bitmap+= ll >= isovalue;
  bitmap <<= 1;
  bitmap += lr >= isovalue;

  double avg = (ul + ur + ll + lr ) / 4;

  if (bitmap == 0b0110 && avg < isovalue) {
    bitmap = 0b1001;
  }
  else if (bitmap == 0b1001 && avg < isovalue) {
    bitmap = 0b0110;
  }

  // std::cout << "Done" << std::endl;

  // std::cout << std::hex << std::to_string(bitmap) << std::endl;
  // always FROM nearest black
  
  Vertex v;

  // size_t length;
  size_t start;
  size_t end;
  
  switch (bitmap) {
    case 0b1110: ;
    case 0b0001: ;
      // line in lower right corner
      // between cols on lrow
      v = Vertex(lcol + lrowmid, lrow);
      start = insertvertex(v);

      v = Vertex(rcol, urow + rcolmid);
      end = insertvertex(v);

      // on rcol between rows

      lines.push_back(Line(start, end));
      break;
    case 0b1101: ;
    case 0b0010: ;
      // line in lower left corner
      // between cols on lrow
      v = Vertex(lcol + lrowmid, lrow);
      start = insertvertex(v);

      // on lcol between rows
      v = Vertex(lcol, urow + lcolmid);
      end = insertvertex(v);

      lines.push_back(Line(start, end));
      break;
    case 0b1011: ;
    case 0b0100: ;
      // line in upper right corner
      // between cols on urow
      v = Vertex(lcol + urowmid, urow);
      start = insertvertex(v);

      // on rcol between rows
      v = Vertex(rcol, urow + rcolmid);
      end = insertvertex(v);

      lines.push_back(Line(start, end));
      break;
    case 0b0111: ;
    case 0b1000: ;
      // line in upper left corner
      // on urow between cols
      v = Vertex(lcol + urowmid, urow);
      start = insertvertex(v);

      // on lcol between rows
      v = Vertex(lcol, urow + lcolmid);
      end = insertvertex(v);

      lines.push_back(Line(start, end));
      break;
    case 0b1100: ;
    case 0b0011: ;
      // horizontal line
      // on lcol between rows
      v = Vertex(rcol, urow + rcolmid);
      start = insertvertex(v);
      
      // on rcol between rows
      v = Vertex(lcol, urow + lcolmid);
      end = insertvertex(v);

      lines.push_back(Line(start, end));
      break;
    case 0b1010: ;
    case 0b0101: ;
      // vertical line
      // on urow between cols
      v = Vertex(lcol + urowmid, urow);
      start = insertvertex(v);

      // between cols on lrow
      v = Vertex(lcol + lrowmid, lrow);
      end = insertvertex(v);

      lines.push_back(Line(start, end));
      break;
    case 0b0110: ;
      // two lines, slanted right
      // line in upper left corner
      // on urow between cols
      
      v = Vertex(lcol, urow + lcolmid);
      start = insertvertex(v);

      // on lcol between rows
      v = Vertex(lcol + urowmid, urow);
      end = insertvertex(v);

      lines.push_back(Line(start, end));
      // line in lower right corner
      // between cols on lrow
      v = Vertex(rcol, urow + rcolmid);
      start = insertvertex(v);

      // on rcol between rows
      v = Vertex(lcol + lrowmid, lrow);
      end = insertvertex(v);

      lines.push_back(Line(start, end));
      break;
    case 0b1001: ;
      // two lines, slanted left
      // line in upper right corner
      // between cols on urow
      v = Vertex(rcol, urow + rcolmid);
      start = insertvertex(v);

      // on rcol between rows
      v = Vertex(lcol + urowmid, urow);
      end = insertvertex(v);

      lines.push_back(Line(start, end));

      // line in lower left corner
      // between cols on lrow
      v = Vertex(lcol, urow + lcolmid);
      start = insertvertex(v);

      // on lcol between rows
      v = Vertex(lcol + lrowmid, lrow);
      end = insertvertex(v);

      lines.push_back(Line(start, end));
      break;
    case 0b1111: ;
    case 0b0000: ;
    default: ;
  }
}

/*
void Pixelmap::interpolate() {

  std::vector<Vertex> verticescopy(vertices);

  for (Line line0 : lines) {
    // get line where line0.start = line1.end

    // std::cout << line0.to_string() << std::endl;
    for (Line line1 : lines) {

      if (line0 == line1)
        continue;

      if (line0.end == line1.start || line0.end == line1.end) {
        // if the lines are connected
        // AND the lines are not the same
        // std::cout << line0.to_string() << " " <<  line1.to_string() << std::endl;
        size_t start = line0.start;
        size_t mid = line0.end;
        size_t end = line0.end == line1.start ? line1.end : line1.start;
        // size_t end = line1.end;
        // std::cout << std::to_string(start) << " " << std::to_string(mid) << " " << std::to_string(end) << std::endl;

        Vertex vstart = verticescopy[start - 1];
        Vertex vend = verticescopy[end - 1];

        Vertex vmid = (vstart + vend) / 2;

        vertices[mid - 1] = vmid;
        break;
      }
    }
  }
}
*/

std::pair<std::vector<Vertex>, std::vector<Line>> Pixelmap::march(double isovalue) {

  // for every set of 4 points:
  //   compare those to cases
  //   move edge points
  //   ? add midpoints ?
  
  for (size_t row = 1; row < height; row++) {
    for (size_t column = 1; column < width; column++) {
      double upleft = pixels[row - 1][column - 1];
      double upright = pixels[row - 1][column];
      double lowleft = pixels[row][column - 1];
      double lowright = pixels[row][column];

      std::vector<double> brightness = {upleft, upright, lowleft, lowright};
      std::vector<size_t> position = {column - 1, column, row - 1, row};

      mapsquare(brightness, position, isovalue);

      // std::cout << curpix << std::endl;
    }
  }

  // interpolate(vertices, lines);
  // interpolate();
  
  for (Vertex v : vertices) {
    std::cout << v.to_string() << "\n";
  }
  

  for (Line l : lines) {
    std::cout << l.to_string() << "\n";
  }

  std::cout << std::flush;

  return std::pair<std::vector<Vertex>, std::vector<Line>>(vertices, lines);
}

std::pair<std::vector<Vertex>, std::vector<Line>> Pixelmap::march(uint8_t isovalue) {

  double newisoval = (double) isovalue / 255;

  return this->march(newisoval);

}

std::string Pixelmap::to_string() {
  std::string str = "";
  for (auto row : pixels) {
    for (auto pixel : row ) {
      // cout << pixel << " ";
      str += std::to_string(pixel) + " ";
    }
    str += "\n";
  }
  return str;
}
