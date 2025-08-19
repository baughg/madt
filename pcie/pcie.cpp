// pcie.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <cassert>
#include "../madt/FileIO.h"


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
  char creator_revision[4];
  uint64_t reserved;
};

struct configuration_space {
  uint64_t base_address;
  uint16_t pci_segment_group_number;
  uint8_t start_pci_bus_num;
  uint8_t end_pci_bus_num;
  uint32_t reserved;
};

struct pci_config_header {
  uint32_t vendor_id : 16;
  uint32_t device_id : 16;
  uint32_t command : 16;
  uint32_t status : 16;
  uint32_t revision_id : 8;
  uint32_t prog_if : 8;
  uint32_t subclass : 8;
  uint32_t class_code : 8;

  uint32_t cache_line_size : 8;
  uint32_t latency_timer : 8;
  uint32_t header_type : 8;
  uint32_t bist : 8;
};

struct pci_config_0 {
  pci_config_header header;
  uint32_t bar0;
  uint32_t bar1;
  uint32_t bar2;
  uint32_t bar3;
  uint32_t bar4;
  uint32_t bar5;
  uint32_t cardbus_cis_pointer;
  uint32_t subsystem_vendor_id : 16;
  uint32_t subsystem_id : 16;
  uint32_t expansion_rom_base_address;
  uint32_t capabilities_pointer : 8;
  uint32_t reserved_0 : 24;
  uint32_t reserver_1;
  uint32_t interrupt_line : 8;
  uint32_t interrupt_pin : 8;
  uint32_t min_grant : 8;
  uint32_t max_latency : 8;
};

int main()
{
  {
    std::vector<uint8_t> buffer;
    std::string dump_filename{ "C:\\development\\assembly\\dump\\pcie.bin" };

    std::deque<configuration_space> configuration_space_list;

    if (FileIO::read(dump_filename, buffer)) {
      auto p_buffer{ buffer.data() };
      auto p_madt{ reinterpret_cast<madt_header*>(p_buffer) };
      int32_t remaining_bytes{ static_cast<int32_t>(p_madt->length) };
      auto total_bytes{ remaining_bytes };

      remaining_bytes -= static_cast<int32_t>(sizeof(madt_header));
      auto p_cur{ p_buffer };
      p_cur += sizeof(madt_header);

      while (remaining_bytes > 0) {
        auto& config{ *reinterpret_cast<configuration_space*>(p_cur) };
        configuration_space_list.push_back(config);
        p_cur += sizeof(configuration_space);
        remaining_bytes -= (static_cast<int32_t>(sizeof(configuration_space)));
      }
    }
  }
  {
    std::vector<uint8_t> buffer;
    std::string dump_filename{ "C:\\development\\assembly\\dump\\pcie_desc.bin" };
    std::deque<pci_config_0> pci_config_0_list;
    
    if (FileIO::read(dump_filename, buffer)) {
      auto p_buffer{ buffer.data() };
      auto& config_header{*reinterpret_cast<pci_config_header*>(p_buffer)};
      uint32_t used_bytes{};

      switch (config_header.header_type) {
      case 0:
      {
        auto& config{ *reinterpret_cast<pci_config_0*>(p_buffer) };
        pci_config_0_list.push_back(config);
        used_bytes += static_cast<uint32_t>(sizeof(pci_config_0));
      }
        break;
      }

      int a = 0;
      a = 1;
    }
  }
}

