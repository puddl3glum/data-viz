#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
// #include <sys/sysinfo.h>
#include <cstdint>
// #include <string.h>
// #include <math.h>
#include <fstream>
#include <iostream>
#include <vector>

#include "vertex.hpp"
#include "pixelmap.hpp"

/*
// typedefs
typedef struct {
  size_t len;
  double** vertices;
} vertices_t;
*/

/*
typedef struct {
  size_t len;
  size_t** lines;
} lines_t;
*/

/*
typedef struct {
  size_t width;
  size_t height;
  double** pixels;
} pixelmap_t;
*/

// forward declarations
// void search(vertices_t*, double*, size_t**);
// void insertvertex(vertices_t*, double*, size_t**);
// void march(const uint8_t*, const size_t, const size_t, vertices_t*, lines_t*);
// void freevertices(vertices_t);
// void freelines(lines_t);
// void printvertices(vertices_t);
// void printlines(lines_t);
// void insertvertices(vertices_t*, lines_t*, double*, double*);
// void interpolate(vertices_t*, lines_t*);
// double distance(double, double, double, double);

using namespace std;

int main(int argc, char* argv[]) {

  char* filename;
  char opt;

  FILE* infile = stdin;

  uint8_t isovalue = 127;

  while ((opt = getopt(argc, argv, "f:i:")) != -1) {
    switch (opt) {
      case 'f': 
        filename = optarg;
        infile = fopen(filename, "rb");
        break;
      case 'i':
        isovalue = atof(optarg);
      default:
        // printf("USAGE: %s\n", argv[0]);
        break;
    }
  }

   if (optind >= argc) {
    cerr << "Expected arguments after options" << endl;;
    exit(EXIT_FAILURE);
  }

  const size_t width = strtoul(argv[0 + optind], NULL, 10);
  const size_t height = strtoul(argv[1 + optind], NULL, 10);

  // calloc buffer
  uint8_t* data = ( uint8_t* ) calloc(height * width, sizeof(uint8_t));

  fread(data, sizeof(uint8_t), height * width, infile);
  fclose(infile);

  // pixelmap_t pixelmap;
  Pixelmap pixelmap(data, width, height);

  free(data);

  pixelmap.march(isovalue);

  // cout << pixelmap.to_string();
 
  // apply threshold
  // # pragma omp parallel for num_threads(get_nprocs()) collapse(2)

  // vertices_t vertices = {0, NULL};
  // lines_t lines = {0, NULL};
  // vector<Vertex> vertexvec;

  // march(data, width, height, &vertices, &lines);

  // connect(&vertices);

  // interpolate(&vertices, &lines);

  // printvertices(vertices);
  // printlines(lines);

  // cleanup
  // free(data);
  // freevertices(vertices);
  // freelines(lines);
}

/*
void printvertices(vertices_t vertices) {
  for (size_t x = 0; x < vertices.len; x++) {
    double* vertex = vertices.vertices[x];
    printf("v %f %f 0\n", vertex[0], vertex[1]);
  }
}
*/

/*
void printlines(lines_t lines) {
  for (size_t x = 0; x < lines.len; x++) {
    size_t* line = lines.lines[x];
    printf("l %d %d\n", line[0], line[1]);
  }
}
*/

/*
void freevertices(vertices_t vertices) {
  for (size_t x = 0; x < vertices.len; x++) {
    free(vertices.vertices[x]);
  }

  free(vertices.vertices);
}
*/

/*
void freelines(lines_t lines) {
  for (size_t x = 0; x < lines.len; x++) {
    free(lines.lines[x]);
  }

  free(lines.lines);
}
*/

/*
void search(vertices_t* vertices, double vertex[], size_t** index) {
  // searches array for vertex pair
  // if found, value at index is set to index, else unchanged
  // printf("%d\n", vertices->len);
  
  for (size_t x = 0; x < vertices->len; x++) {
    double* avertex = vertices->vertices[x];
    // printf("(%f, %f) ?= (%f, %f)\n", vertex[0], vertex[1], avertex[0], avertex[1]);
    if (avertex[0] == vertex[0] && avertex[1] == vertex[1]) {
      // index expected to be null
      // allocate for it and set
      *index = ( size_t* ) calloc(1, sizeof(size_t));
      **index = x;
    }
  }
}
*/

/*
void insertvertex(vertices_t* vertices, double vertex[], size_t** index) {

  search(vertices, vertex, index);
  // puts("DONE");

  if (*index == NULL) {
    // not found
    // extend array and insert at end
    vertices->len++;
    // printf("vertices: %d\n", vertices->len);

    double** temp = vertices->vertices;
    vertices->vertices = ( double** ) calloc(vertices->len, sizeof(double*));
    memcpy(vertices->vertices, temp, sizeof(double*) * (vertices->len - 1));
    free(temp);
    
    vertices->vertices[vertices->len - 1] = vertex;

    *index = ( size_t* ) calloc(1, sizeof(size_t));
    **index = vertices->len - 1;
  }
  else {
    // vertex already contained
    // free this one
    free(vertex);
  }
}
*/

/*
void insertline(lines_t* lines, size_t line[]) {
  *
   * Inserts a line into the last index
   * *

  lines->len++;

  size_t** temp = lines->lines;
  lines->lines = ( size_t** ) calloc(lines->len, sizeof(size_t*));
  memcpy(lines->lines, temp, sizeof(size_t*) * (lines->len - 1));
  free(temp);

  lines->lines[lines->len - 1] = line;
}
*/

/*
void insertvertices(vertices_t* vertices, lines_t* lines, double* vertex0, double* vertex1) {

  size_t** index = ( size_t** ) calloc(1, sizeof(size_t*));

  size_t* line = ( size_t* ) calloc(2, sizeof(size_t));

  *index = NULL;
  insertvertex(vertices, vertex0, index);
  line[0] = **index + 1;
  free(*index);

  *index = NULL;
  insertvertex(vertices, vertex1, index);
  line[1] = **index + 1;
  free(*index);

  insertline(lines, line);

  free(index);
}
*/

/*
void march(const uint8_t* data, const size_t width, const size_t height, vertices_t* vertices, lines_t* lines) {
  *
   * Sets vertices and lines to vertices and lines pointers.
   * The values, if these pointers contain any, will be overwritten
   * *

  // returns the vertices and lines after marching
  // vertices are stored at vertices pointer (which will be clobbered)
  // lines are stored at the lines point (which will be clobbered)

  // vertices = calloc(0, sizeof(double*));
  // size_t verticeslen = 0;

  // March through
  // # pragma omp parallel for num_threads(get_nprocs()) collapse(2)
  for (size_t row = 1; row < height; row++) {
    for (size_t column = 1; column < width; column++ ) {
      // printf("%d ", data[row * height + column]);
      uint8_t square = 0;
      // top left
      square += data[(row - 1) * height + (column - 1)];
      // top right 
      square <<= 1;
      square += data[(row - 1) * height + (column)];
      // bottom left
      square <<= 1;
      square += data[(row) * height + (column - 1)];
      // bottom right
      square <<= 1;
      square += data[(row) * height + (column)];
      // square is: <topl><topr><botl><botr>
      // (square != 0 && square != 0xf) ? printf("%x", square) : printf(" ");;

      // top row
      double tr = row - 1;
      // bottom row
      double br = row;
      // left colum
      double lc = column - 1;
      // right column
      double rc = column;

      double* right;
      double* left;
      double* top;
      double* bottom;

      // store last position and go from there

      switch (square) {
        // both of these are the same
        case 0b1110: ;
        case 0b0001: ; // blank statement after label /shrug
          // diagonal bottom right
          // one third from bottom on right
          right = ( double* ) calloc(2, sizeof(double));
          right[0] = rc;
          right[1] = br - 0.5;

          bottom = ( double* ) calloc(2, sizeof(double));
          bottom[0] = rc - 0.5;
          bottom[1] = br;

          insertvertices(vertices, lines, right, bottom);

          break;
        case 0b1101: ;
        case 0b0010: ;
          // diagonal bottom left
          // one third from bottom on left
          left = ( double*  ) calloc(2, sizeof(double));
          left[0] = lc;
          left[1] = br - 0.5;

          // bottom
          // one third from left on bottom
          bottom = ( double* ) calloc(2, sizeof(double));
          bottom[0] = lc + 0.5;
          bottom[1] = br;

          insertvertices(vertices, lines, left, bottom);

          break;
        case 0b0111: ;
        case 0b1000: ;
          // diagonal top left
          // one third from top on left
          left = ( double* ) calloc(2, sizeof(double));
          left[0] = lc;
          left[1] = tr + 0.5;

          // one third from left on top
          top = ( double* ) calloc(2, sizeof(double));
          top[0] = lc + 0.5;
          top[1] = tr;

          insertvertices(vertices, lines, left, top);

          break;
        case 0b1011: ;
        case 0b0100: ;
          // diagonal top right
          // one third from top on right
          right = ( double* ) calloc(2, sizeof(double));
          right[0] = rc;
          right[1] = tr + 0.5;

          // one third from right on top
          top = ( double* ) calloc(2, sizeof(double));
          top[0] = rc - 0.5;
          top[1] = tr;

          insertvertices(vertices, lines, right, top);

          break;
        case 0b1100: ;
        case 0b0011: ;
          // horizontal line
          // left halway from bottom
          left = ( double* ) calloc(2, sizeof(double));
          left[0] = lc;
          left[1] = br - 0.5;

          right = ( double* ) calloc(2, sizeof(double));
          right[0] = rc;
          right[1] = br - 0.5;

          insertvertices(vertices, lines, left, right);

          break;
        case 0b1010: ;
        case 0b0101: ;
          // vertical line
          top = ( double* ) calloc(2, sizeof(double));
          top[0] = lc + 0.5;
          top[1] = tr;

          bottom = ( double* ) calloc(2, sizeof(double));
          bottom[0] = lc + 0.5;
          bottom[1] = br;

          insertvertices(vertices, lines, top, bottom);

          break;
        case 0b0110: ;
          // diagonal top left
          // one third on top from left
          top = ( double* ) calloc(2, sizeof(double));
          top[0] = lc + 0.5;
          top[1] = tr;

          // one third on left from top
          left = ( double* ) calloc(2, sizeof(double));
          left[0] = lc;
          left[1] = tr + 0.5;

          insertvertices(vertices, lines, top, left);

          // diagonal bottom right
          // one third on bottom from right
          bottom = ( double* ) calloc(2, sizeof(double));
          bottom[0] = rc - 0.5;
          bottom[1] = br;

          // one third on right from bottom
          right = ( double* ) calloc(2, sizeof(double));
          right[0] = rc;
          right[1] = br - 0.5;

          insertvertices(vertices, lines, bottom, right);
          break;
        case 0b1001: ;
          // diagonal top right
          // one third from top on right
          right = ( double* ) calloc(2, sizeof(double));
          right[0] = rc;
          right[1] = tr + 0.5;

          // one third from right on top
          top = ( double* ) calloc(2, sizeof(double));
          top[0] = rc - 0.5;
          top[1] = tr;

          insertvertices(vertices, lines, right, top);

          // diagonal bottom left
          // one third from bottom on left
          left = ( double* ) calloc(2, sizeof(double));
          left[0] = lc;
          left[1] = br - 0.5;

          // bottom
          // one third from left on bottom
          bottom = ( double* ) calloc(2, sizeof(double));
          bottom[0] = lc + 0.5;
          bottom[1] = br;

          insertvertices(vertices, lines, left, bottom);

          break;
        case 0b0000: ;
        case 0b1111: ;
        default: ;
          break;
      }
    }
  }
}
*/


/*
void interpolate(vertices_t* vertices, lines_t* lines) {

  if (lines->len == 0)
    return;

  for (size_t x = 0; x < lines->len - 1; x++) {

    // line vertices are index + 1
    size_t* line = lines->lines[x];

    size_t first = line[0];
    size_t second = line[1];
    size_t third;


    for (size_t y = 1; y < lines->len; y++) {

      size_t* templine = lines->lines[y];

      if (second == templine[0]) {
        third = templine[1];
        // printf("%d %d %d %d\n", first, second, templine[0], third);
        double* vbeg = vertices->vertices[first - 1];
        double* vmid = vertices->vertices[second - 1];
        double* vend = vertices->vertices[third - 1];

        double x = (vbeg[0] + vend[0]) / 2;
        double y = (vbeg[1] + vend[1]) / 2;

        // printf("(%.2f, %.2f), (%.2f, %.2f)->(%.2f, %.2f), (%.2f, %.2f)\n", vbeg[0], vbeg[1], vmid[0], vmid[1], x, y, vend[0], vend[1]);

        vmid[0] = x;
        vmid[1] = y;
        break;
      }
    }


    // printf("(%f, %f)\n", vmid[0], vmid[1]);
  }

  // exit(0);

}
*/

