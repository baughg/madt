// text2bin.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <cstdint>
#include <fstream>
#include <string>
#include <deque>
#include "../madt/FileIO.h"

std::vector<uint8_t> text2bin(const std::string& filename);

int main()
{
  auto data{ text2bin("C:\\development\\assembly\\dump\\npu_pci.txt")};

  FileIO::write("C:\\development\\assembly\\dump\\npu_pci.bin", data.data(), data.size());
}


std::vector<uint8_t> text2bin(const std::string& filename) {
  std::ifstream text_file{ filename };
  std::deque<uint64_t> values;

  if (!text_file.good()) {
    return std::vector<uint8_t>{};
  }
  std::string line_in;

  while (!text_file.eof()) {
    uint64_t value{};
    text_file >> std::hex >> value;
    //getline(text_file, line_in);

    values.push_back(value);
  }
  text_file.close();

  std::vector<uint8_t> bin(values.size() << 3);

  auto p_data_64{ reinterpret_cast<uint64_t*>(bin.data()) };

  for (auto v : values) {
    *p_data_64++ = v;
  }

  return bin;
}
