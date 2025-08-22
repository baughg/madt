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



union base_address {
  uint32_t address;

  struct base_address_bf {
    uint32_t always_0 : 1;
    uint32_t type : 2;
    uint32_t prefetchable : 1;
    uint32_t aligned_base_address : 28;
  };

  base_address_bf address_bf;
};

struct pci_config_0 {
  pci_config_header header;
  base_address bar0;
  base_address bar1;
  base_address bar2;
  base_address bar3;
  base_address bar4;
  base_address bar5;
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

struct pci_config_header_0 {
  pci_config_0 config;
  uint64_t bar[6] = {};
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
    std::string dump_filename{ "C:\\development\\assembly\\dump\\npu_pci.bin" };
    std::deque<pci_config_header_0> pci_config_0_list;
    
    if (FileIO::read(dump_filename, buffer)) {
      auto p_buffer{ buffer.data() };
      auto& config_header{*reinterpret_cast<pci_config_header*>(p_buffer)};
      uint32_t used_bytes{};

      switch (config_header.header_type) {
      case 0:
      {
        auto& config{ *reinterpret_cast<pci_config_0*>(p_buffer) };
        pci_config_header_0 config_ex{};
        config_ex.config = config;

        if (config.bar0.address_bf.type == 0x2) {
          uint64_t bar0{ static_cast<uint64_t>(config.bar0.address & 0xfffffff0u) | (static_cast<uint64_t>(config.bar1.address & ~0u) << 32) };          
          config_ex.bar[0] = bar0;
        }
        if (config.bar4.address_bf.type == 0x2) {
          uint64_t bar4{ static_cast<uint64_t>(config.bar4.address & 0xfffffff0u) | (static_cast<uint64_t>(config.bar5.address & ~0u) << 32) };          
          config_ex.bar[4] = bar4;
        }
        pci_config_0_list.push_back(config_ex);
        used_bytes += static_cast<uint32_t>(sizeof(pci_config_0));
      }
        break;
      }

      int a = 0;
      a = 1;
    }
  }
}

