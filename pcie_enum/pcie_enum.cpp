// pcie.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <cassert>
#include "../madt/FileIO.h"

void show_physical_address(std::vector<uint64_t>& offsets) {
  uint32_t index{};

  for (auto offset : offsets) {
    /*
    * push rdx			; Save RDX for the register
  and edx, 0xFFFF0000		; Isolate the device/function
  shr edx, 4			; Quick divide by 16
  add rsi, rdx			; RSI now points to the start of the 4KB register memory
  pop rdx				; Low 10 bits of RDX is the register
  and edx, 0x000003FF		; Only keep the low 10 bits
  shl edx, 2			; Quick multiply by 4
  add rsi, rdx			; Add offset for the register
  mov rdx, rsi			; Store final memory address in RDX
    */
    uint64_t dev_func{ offset & 0xFFFF0000ull };
    dev_func >>= 4;
    dev_func += 0xC0000000ull;
    uint64_t lower_10b{ offset & 0x000003FFull };
    lower_10b <<= 2;
    dev_func += lower_10b;

    std::cout << std::dec << index << "\t" << std::hex << "0x" << dev_func << "\n";
    index++;
  }

  std::cout << "done\n";
}

int main()
{

  std::vector<uint8_t> buffer;
  std::string dump_filename{ "C:\\development\\assembly\\dump\\pcie_enum_offset.bin" };


  if (FileIO::read(dump_filename, buffer)) {

    auto p_buffer{ buffer.data() };
    auto& count{ *reinterpret_cast<uint64_t*>(p_buffer) };
    p_buffer += sizeof(uint64_t);

    std::vector<uint64_t> offsets;
    offsets.reserve(count);

    for (uint64_t i{}; i < count; ++i) {
      auto& val{ *reinterpret_cast<uint64_t*>(p_buffer) };
      offsets.push_back(val);
      p_buffer += sizeof(uint64_t);
    }

    show_physical_address(offsets);
  }

}

