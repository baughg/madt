// idt.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdint>

#pragma pack(push, 1)
union idt_entry_full {
  uint64_t idt_entry_value;

  struct idt_entry {
    uint16_t offset_0_15;
    uint16_t segment_selector; // 2
    uint8_t reserved; // 4
    uint8_t gate_type : 4; // 5
    uint8_t zero : 1;
    uint8_t DPL : 2;
    uint8_t P : 1;
    uint16_t offset_16_31;
  };

  idt_entry idt_entry_bf;
};

int main()
{
  idt_entry_full entry{};
  entry.idt_entry_value = 0x108f0000084610ull;

  int a = 0;
  a = 1;
}

