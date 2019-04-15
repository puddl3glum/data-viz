
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "getopt.h"

template <typename T>
std::vector<T> read_data(std::istream& in) {
  
  in.seekg(0, std::ios::end);
  size_t filesize = in.tellg();
  in.seekg(0, std::ios::beg);

  std::vector<T> data;

  data.resize(filesize / sizeof(T));

  in.read((char *) data.data(), filesize);

  return data;
}

int main(int argc, char* argv[]) {

  int flags, opt;

  std::string filename = "";
  int extent = 0;
  std::string shape = "";

  while(getopt(argc, argv, "f:e:s:") != -1) {


    switch (opt) {

      case 'f':
        filename = optarg;
        break;
    }
  }

  std::vector<uint8_t> data;

  if (filename != "") {
    std::ifstream instream(filename, std::ios::in);
    data = read_data<uint8_t>(instream);
  }

  for (auto d : data) {
    std::cout << d << std::endl;
  }
}

