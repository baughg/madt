// madt.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <cassert>
#include "FileIO.h"

#pragma pack(push, 1)
struct madt_header {
  char signature[4];
  uint32_t length;
  uint8_t revision;
  uint8_t checksum;
  uint8_t oemid[6];
  char oem_table_id[8];
  uint32_t oem_revision;
  char creator_id[4];
  uint32_t creator_revision;
  uint32_t local_APIC_address;
  uint32_t flags;
};

#pragma pack(push, 1)
struct apic_entry {
  uint8_t entry_type;
  uint8_t record_length;
};

struct processor_local_apic {
  uint8_t acpi_processor_id;
  uint8_t apic_id;
  uint32_t processor_enabled : 1;
  uint32_t online_capable : 1;
  uint32_t unused : 30;
};

struct io_apic {
  uint8_t io_apic_id;
  uint8_t reserved;
  uint32_t io_apic_address;
  uint32_t global_system_interrupt_base;
};

struct io_apic_interrupt_source_override {
  uint8_t bus_source;
  uint8_t irq_source;
  uint32_t global_system_interrupt;
  uint16_t flags;
};

struct local_apic_non_maskable_interrupts {
  uint8_t acpi_processor_id; // 0xff means all processors
  uint16_t flags;
  uint8_t lint_num; // 0 or 1
};

struct processor_local_x2APIC {
  uint16_t reserved;
  uint32_t processor_local_x2apic_id;
  uint32_t flags;
  uint32_t acpi_id;
};

int main()
{
  std::vector<uint8_t> buffer;
  std::string dump_filename{ "C:\\development\\assembly\\dump\\madt-0-256B_manual.bin" };
  std::deque<processor_local_apic> processor_local_apic_list;
  std::deque<io_apic> io_apic_list;
  std::deque<io_apic_interrupt_source_override> io_apic_interrupt_source_override_list;
  std::deque<local_apic_non_maskable_interrupts> local_apic_non_maskable_interrupts_list;
  std::deque<processor_local_x2APIC> processor_local_x2APIC_list;

  if (FileIO::read(dump_filename, buffer)) {
    auto p_buffer{ buffer.data() };
    auto p_madt{ reinterpret_cast<madt_header*>(p_buffer) };
    int32_t remaining_bytes{ static_cast<int32_t>(p_madt->length) };
    auto total_bytes{ remaining_bytes };

    remaining_bytes -= static_cast<int32_t>(sizeof(madt_header));
    auto p_cur{ p_buffer };
    p_cur += sizeof(madt_header);

    while (remaining_bytes > 0) {
      auto p_entry{ reinterpret_cast<apic_entry*>(p_cur) };
      p_cur += sizeof(apic_entry);
      remaining_bytes -= static_cast<int32_t>(p_entry->record_length);
      auto offset{ total_bytes - remaining_bytes };

      if (offset > 850) {
        int a = 0;
        a = 1;
      }
      switch (p_entry->entry_type) {
      case 0:
      {
        auto& ent_type{ *reinterpret_cast<processor_local_apic*>(p_cur) };
        p_cur += sizeof(processor_local_apic);
        processor_local_apic_list.push_back(ent_type);
      }
      break;
      case 1:
      {
        auto& ent_type{ *reinterpret_cast<io_apic*>(p_cur) };
        p_cur += sizeof(io_apic);
        io_apic_list.push_back(ent_type);
      }
        break;
      case 2:
      {
        auto& ent_type{ *reinterpret_cast<io_apic_interrupt_source_override*>(p_cur) };
        p_cur += sizeof(io_apic_interrupt_source_override);
        io_apic_interrupt_source_override_list.push_back(ent_type);        
      }
      break;
      case 4:
      {        
        auto& ent_type{ *reinterpret_cast<local_apic_non_maskable_interrupts*>(p_cur) };
        p_cur += sizeof(local_apic_non_maskable_interrupts);
        local_apic_non_maskable_interrupts_list.push_back(ent_type);
      }
      break;
      case 9:
      {
        auto& ent_type{ *reinterpret_cast<processor_local_x2APIC*>(p_cur) };
        p_cur += sizeof(processor_local_x2APIC);
        processor_local_x2APIC_list.push_back(ent_type);
      }
      break;
      default:
        assert(false);
        break;
      }
    }    
  }
}

