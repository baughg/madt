#pragma once
#include <string>
#include <cstdint>
#include <vector>
#include <fstream>

class FileIO
{
public:
  static bool write(const std::string& filename,
    const void* p_data, const uint32_t& size);
  static bool read(const std::string& filename,
    std::vector<uint8_t>& input_data);
  static void write_hex_buffer(std::ofstream& hex_file,
    std::vector<uint8_t>& buffer);
};

