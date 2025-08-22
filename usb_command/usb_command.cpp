// pcie.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <cassert>
#include <iomanip>
#include <sstream>
#include "../madt/FileIO.h"


std::string print_hex(uint32_t val, const uint32_t& nibbles) {
  std::stringstream ss;
  ss << "0x" << std::hex << std::uppercase << std::setw(nibbles) << std::setfill('0') << val << std::dec;
  return ss.str();
}

std::string TRB_type_name(uint32_t trb_type) {
  std::string description;

  switch (trb_type) {
  case 0:
    description = "Reserved";
    break;
  case 1:
    description = "Normal (Transfer Ring)";
    break;
  case 2:
    description = "Setup Stage (Transfer Ring)";
    break;
  case 3:
    description = "Data Stage (Transfer Ring)";
    break;
  case 4:
    description = "Status Stage (Transfer Ring)";
    break;
  case 5:
    description = "Isoch (Transfer Ring)";
    break;
  case 6:
    description = "Link (Transfer/Command Ring)";
    break;
  case 7:
    description = "Event Data (Transfer Ring)";
    break;
  case 8:
    description = "No-Op (Transfer Ring)";
    break;
  case 9:
    description = "Enable Slot Command (Command Ring)";
    break;
  case 10:
    description = "Disable Slot Command (Command Ring)";
    break;
  case 11:
    description = "Address Device Command (Command Ring)";
    break;
  case 12:
    description = "Configure Endpoint Command (Command Ring)";
    break;
  case 13:
    description = "Evaluate Context Command (Command Ring)";
    break;
  case 14:
    description = "Reset Endpoint Command (Command Ring)";
    break;
  case 15:
    description = "Stop Endpoint Command (Command Ring)";
    break;
  case 16:
    description = "Set TR Dequeue Pointer Command (Command Ring)";
    break;
  case 17:
    description = "Reset Device Command (Command Ring)";
    break;
  case 18:
    description = "Force Event Command (Optional, used with virtualization only) (Command Ring)";
    break;
  case 19:
    description = "Negotiate Bandwidth Command (Optional) (Command Ring)";
    break;
  case 20:
    description = "Set Latency Tolerance Value Command (Optional) (Command Ring)";
    break;
  case 21:
    description = "Get Port Bandwidth Command (Optional) (Command Ring)";
    break;
  case 22:
    description = "Force Header Command (Command Ring)";
    break;
  case 23:
    description = "No Op Command (Command Ring)";
    break;
  case 24:
    description = "Get Extended Property Command (Optional) (Command Ring)";
    break;
  case 25:
    description = "Set Extended Property Command (Optional) (Command Ring)";
    break;
  case 26:
  case 27:
  case 28:
  case 29:
  case 30:
  case 31:
    description = "Reserved";
    break;
  case 32:
    description = "Transfer Event (Event Ring)";
    break;
  case 33:
    description = "Command Completion Event (Event Ring)";
    break;
  case 34:
    description = "Port Status Change Event (Event Ring)";
    break;
  case 35:
    description = "Bandwidth Request Event (Event Ring)";
    break;
  case 36:
    description = "Doorbell Event (Optional, used with virtualization only) (Event Ring)";
    break;
  case 37:
    description = "Host Controller Event (Event Ring)";
    break;
  case 38:
    description = "Device Notification Event (Event Ring)";
    break;
  case 39:
    description = "MFINDEX Wrap Event (Event Ring)";
    break;
  case 40:
  case 41:
  case 42:
  case 43:
  case 44:
  case 45:
  case 46:
  case 47:
    description = "Reserved";
    break;
  default:
    description = "Vendor Defined";
  }

  return description;
}
#pragma pack(push, 1)
struct completion_trb_bf {
  /*uint32_t RsvdZ : 4;
  uint32_t command_trb_pointer_lo : 28;
  uint32_t command_trb_pointer_hi;
  uint32_t command_completion_parameter : 24;
  uint32_t completion_code : 8;
  uint32_t C : 1;
  uint32_t RsvdZ_1 : 9;
  uint32_t trb_type : 6;
  uint32_t vf_id : 8;
  uint32_t slot_id : 8;*/

  /*
  * ; At this point the event ring should contain some port status change event entries
  ; They should appear as follows:
  ; 0xXX000000 0x00000000 0x01000000 0x00008801
  ; dword 0 - Port ID number (31:24)
  ; dword 1 - Reserved
  ; dword 2 - Completion code (31:24)
  ; dword 3 - Type 34 (15:10), C (0)
  */
  uint32_t reserved_0 : 24;
  uint32_t port_ID_num : 8;
  uint32_t reserved_1;
  uint32_t reserved_2 : 24;
  uint32_t completion_code : 8;
  uint32_t C : 1;
  uint32_t RsvdZ_1 : 9;
  uint32_t trb_type : 6;
  uint32_t vf_id : 8;
  uint32_t slot_id : 8;
};

union ring_status {
  uint64_t val;
  struct ring_status_bf {
    uint32_t reserved_2 : 24;
    uint32_t completion_code : 8;
    uint32_t C : 1;
    uint32_t RsvdZ_1 : 9;
    uint32_t trb_type : 6;
    uint32_t vf_id : 8;
    uint32_t slot_id : 8;
  };

  ring_status_bf bitfield;
};

union completion_trb {
  uint64_t words[2];
  completion_trb_bf bitfield;
};

union setup_stage_trb {
  uint32_t words[4];

  void print() {
    std::cout << "; setup\n";
    for (uint32_t i{}; i < 4; ++i) {
      std::cout << "mov eax, 0x" << std::hex << words[i] << "\n";
    }
  }
  void print_verbose() {
    std::string description{};
    std::cout << "  ; setup TRB\n";
    std::cout << "  ; ************ DWORD[0] ************\n";
    std::cout << "  ; bmRequestType(7:0)\n";
    std::cout << "  ;  .recipient(4:0)       =" << std::dec << static_cast<uint32_t>(bitfield.request_type.recipient) << " (";
    switch (bitfield.request_type.recipient) {
    case 0:
      description = "Device";
      break;
    case 1:
      description = "Interface";
      break;
    case 2:
      description = "Endpoint";
      break;
    case 3:
      description = "Other";
      break;
    default:
      description = "Reserved";
      break;
    }
    std::cout << description << ")\n";

    std::cout << "  ;  .type_of_request(6:5) =" << std::dec << static_cast<uint32_t>(bitfield.request_type.type_of_request) << " (";

    switch (bitfield.request_type.type_of_request) {
    case 0:
      description = "Standard";
      break;
    case 1:
      description = "Class";
      break;
    case 2:
      description = "Vendor";
      break;
    case 3:
      description = "Reserved";
      break;
    default:
      description = "Undefined";
      break;
    }
    std::cout << description << ")\n";

    std::cout << "  ;  .transfer_direction(7)=" << std::dec << static_cast<uint32_t>(bitfield.request_type.transfer_direction) << " (";

    switch (bitfield.request_type.transfer_direction) {
    case 0:
      description = "Host-to-device";
      break;
    case 1:
      description = "Device-to-host";
      break;
    default:
      description = "Undefined";
      break;
    }
    std::cout << description << ")\n";

    std::cout << "  ; bRequest(15:8)=" << std::hex << print_hex(static_cast<uint32_t>(bitfield.bRequest),2) << " (";

    switch (static_cast<uint32_t>(bitfield.bRequest)) {
    case 0:
      description = "GET_STATUS";
      break;
    case 1:
      description = "CLEAR_FEATURE";
      break;
    case 2:
      description = "Reserved";
      break;
    case 3:
      description = "SET_FEATURE";
      break;
    case 4:
      description = "Reserved";
      break;
    case 5:
      description = "SET_ADDRESS";
      break;
    case 6:
      description = "GET_DESCRIPTOR";
      break;
    case 7:
      description = "SET_DESCRIPTOR";
      break;
    case 8:
      description = "GET_CONFIGURATION";
      break;
    case 9:
      description = "SET_CONFIGURATION";
      break;
    case 10:
      description = "GET_INTERFACE";
      break;
    case 11:
      description = "SET_INTERFACE";
      break;
    case 12:
      description = "SYNC_FRAME";
      break;
    default:
      description = "UnDeFiNeD";
      break;
    };
    std::cout << description << ")\n";
    uint16_t& val{ *reinterpret_cast<uint16_t*>(bitfield.wValue) };
    std::cout << "  ; wValue(31:16)=" << print_hex(static_cast<uint32_t>(val),4) << "\n";
    std::cout << "  ;  [0](23:16)  =" << print_hex(static_cast<uint32_t>(bitfield.wValue[0]),2) << "\n";
    std::cout << "  ;  [1](31:24)  =" << print_hex(static_cast<uint32_t>(bitfield.wValue[1]),2) << "\n";    
    std::cout << "  mov eax, " << print_hex(words[0], 8) << "\n\n";

    std::cout << "  ; ************ DWORD[1] ************\n";
    uint16_t& index{ *reinterpret_cast<uint16_t*>(bitfield.wIndex) };
    std::cout << "  ; wIndex(15:0) =" << print_hex(static_cast<uint32_t>(index), 4) << "\n";
    std::cout << "  ;  [0](7:0)    =" << print_hex(static_cast<uint32_t>(bitfield.wIndex[0]), 2) << "\n";
    std::cout << "  ;  [1](15:8)   =" << print_hex(static_cast<uint32_t>(bitfield.wIndex[1]), 2) << "\n";
    std::cout << "  ; wLength(15:0)=" << print_hex(static_cast<uint32_t>(bitfield.wLength), 4) << " : " 
      << std::dec << static_cast<uint32_t>(bitfield.wLength) << "d\n";
    std::cout << "  mov eax, " << print_hex(words[1], 8) << "\n\n";

    std::cout << "  ; ************ DWORD[2] ************\n";
   
    {
      auto transfer_len{ static_cast<uint32_t>(bitfield.trb_transfer_length) };
      auto interrupter_target{ static_cast<uint32_t>(bitfield.interrupter_target) };
      std::cout << "  ; trb_transfer_length(16:0)=" << print_hex(transfer_len, 5) << std::dec << " : " << transfer_len << "d\n";
      std::cout << "  ; interrupter_target(31:22)=" << print_hex(interrupter_target, 3) << std::dec << " : " << interrupter_target << "d\n";
    }
    std::cout << "  mov eax, " << print_hex(words[2], 8) << "\n\n";

    std::cout << "  ; ************ DWORD[3] ************\n";
    std::cout << "  ; C(0)  =" << std::dec << static_cast<uint32_t>(bitfield.C) << " (Cycle bit)\n";
    std::cout << "  ; IOC(5)=" << std::dec << static_cast<uint32_t>(bitfield.IOC) << " (Interrupt On Completion)\n";
    std::cout << "  ; IDT(6)=" << std::dec << static_cast<uint32_t>(bitfield.IDT) << " (Immediate Data)\n";
    auto trb_type{ static_cast<uint32_t>(bitfield.trb_type) };
    std::cout << "  ; trb_type(15:10)=" << std::dec << trb_type << " (";
    std::cout << TRB_type_name(trb_type) << ")\n";

    std::cout << "  ; trt(17:16)     =" << std::dec << trb_type << " Transfer Type (";

    switch (trb_type) {
    case 0:
      description = "No Data Stage";
      break;
    case 1:
      description = "Reserved";
      break;
    case 2:
      description = "OUT Data Stage";
      break;
    case 3:
      description = "IN Data Stage";
      break;
    default:
      description = "UnDeFiNeD";
      break;
    }

    std::cout << description << ")\n";

    std::cout << "  mov eax, " << print_hex(words[3], 8) << "\n\n";   
  }
  struct setup_stage_trb_bf {
    struct bmRequestType {
      /*
      * 	Recipient
      00000b = Device
      00001b = Interface
      00010b = Endpoint
      00011b = Other
      00100b to 11111b = Reserved
      */
      uint8_t recipient : 5;
      /*
      * Type of request
      00b = Standard
      01b = Class
      10b = Vendor
      11b = Reserved
      */
      uint8_t type_of_request : 2;
      /*
      * Data transfer direction * The value of this bit is ignored when wLength is zero
      0b = Host-to-device
      1b = Device-to-host
      */
      uint8_t transfer_direction : 1;
    };
    bmRequestType request_type;
    uint8_t bRequest;
    // wValue: Word-sized field the may (or may not) serve as a parameter to the request, depending on the specific request.
    uint8_t wValue[2];
    /*
    * wIndex
    Word-sized field that may (or may not) serve as a parameter to the request, depending on the specific request. Typically this field holds an index or an offset value.
    When bmRequestType specifies an endpoint as the recipient, the format of this field is as follows:

    -----------------------------------------
    | D7 | D6 | D5 | D4 | D3 | D2 | D1 | D0 |
    -----------------------------------------
    |dire| reserved     |  endpoint         |
    |ctio| (reset to    |  number           |
    |n   | zero)        |                   |
    -----------------------------------------
    |D15 |D14 |D13 |D12 |D11 |D10 | D9 | D8 |
    -----------------------------------------
    |    reserved (reset to zero)           |
    -----------------------------------------
    The direction bit (bit D7) is set to zero to indicate the OUT endpoint with the specified endpoint number, or it is set to one to indicate the IN endpoint with the specified
    endpoint number. The host should always set the direction bit to zero (but the device should accept either value) when the endpoint is part of a control pipe.
    When bmRequestType specifies an interface as the recipient, the format of this field is as follows:
    -----------------------------------------
    | D7 | D6 | D5 | D4 | D3 | D2 | D1 | D0 |
    -----------------------------------------
    |              interface number         |
    -----------------------------------------
    |D15 |D14 |D13 |D12 |D11 |D10 | D9 | D8 |
    -----------------------------------------
    |    reserved (reset to zero)           |
    -----------------------------------------
    */
    uint8_t wIndex[2];
    /*
    * wLength
    Number of bytes to transfer if there is a DATA stage.

    If this field is non-zero, and bmRequestType indicates a transfer from device-to-host, then the device must never return more than wLength bytes of data. However,
    a device may return less.
    If this field is non-zero, and the bmRequestType indicates a transfer from host-to-device, then the host must send exactly wLength bytes of data.
    If the host sends more than wLength bytes, the behavior of the device is undefined.
    */
    uint16_t wLength;
    struct {
      uint32_t trb_transfer_length : 17;
      uint32_t RsvdZ : 5;
      uint32_t interrupter_target : 10;
    };
    struct {
      uint32_t C : 1; // Cycle bit (C). This bit is used to mark the Enqueue point of a Transfer ring.
      uint32_t RsvdZ_0 : 4;
      uint32_t IOC : 1; /*Interrupt On Completion (IOC). If this bit is set to ‘1’, it specifies that when this TRB completes, the Host Controller shall notify the system of the completion by placing an Event TRB on the Event ring and sending an interrupt at the next interrupt threshold. Refer to section 4.10.4.*/
      uint32_t IDT : 1; /*Immediate Data (IDT). This bit shall be set to ‘1’ in a Setup Stage TRB. It specifies that the Parameter component of this TRB contains Setup Data.*/
      uint32_t RsvdZ_1 : 3;
      uint32_t trb_type : 6;
      uint32_t trt : 2; /*Transfer Type (TRT). This field indicates the type and direction of the control transfer.
                          Value Definition
                          0 No Data Stage
                          1 Reserved
                          2 OUT Data Stage
                          3 IN Data Stage*/
      uint32_t RsvdZ_2 : 14;
    };
  };

  setup_stage_trb_bf bitfield;
};


union data_stage_trb {
  uint32_t words[4];

  void print() {
    std::cout << "; data\n";
    for (uint32_t i{}; i < 4; ++i) {
      std::cout << "mov eax, 0x" << std::hex << words[i] << "\n";
    }
  }

  void print_verbose() {
    std::cout << "  ; data TRB\n";
    std::cout << "  ; ************ DWORD[0] ************\n";
    std::cout << "  ; data_buffer_lo(31:0)=" << print_hex(bitfield.data_buffer_lo, 8) << "\n";
    std::cout << "  ; ************ DWORD[1] ************\n";
    std::cout << "  ; data_buffer_hi(31:0)=" << print_hex(bitfield.data_buffer_hi, 8) << "\n";

    std::cout << "  ; ************ DWORD[2] ************\n";

    {
      auto transfer_len{ static_cast<uint32_t>(bitfield.trb_transfer_length) };
      auto interrupter_target{ static_cast<uint32_t>(bitfield.interrupter_target) };
      auto td_size{ static_cast<uint32_t>(bitfield.td_size) };

      std::cout << "  ; trb_transfer_length(16:0)=" << print_hex(transfer_len, 5) << std::dec << " : " << transfer_len << "d\n";
      std::cout << "  ; td_size(21:17)           =" << print_hex(td_size, 5) << std::dec << " : " << td_size << "d\n";
      std::cout << "  ; interrupter_target(31:22)=" << print_hex(interrupter_target, 3) << std::dec << " : " << interrupter_target << "d\n";
    }
    std::cout << "  mov eax, " << print_hex(words[2], 8) << "\n\n";

    std::cout << "  ; ************ DWORD[3] ************\n";
    std::cout << "  ; C(0)  =" << std::dec << static_cast<uint32_t>(bitfield.C) << " (Cycle bit)\n";
    std::cout << "  ; ENT(1)=" << std::dec << static_cast<uint32_t>(bitfield.ENT) << " (Evaluate Next TRB)\n";
    std::cout << "  ; ISP(2)=" << std::dec << static_cast<uint32_t>(bitfield.ISP) << " (Interrupt-on Short Packet)\n";
    std::cout << "  ; NS(3) =" << std::dec << static_cast<uint32_t>(bitfield.NS) << " (No Snoop)\n";
    std::cout << "  ; CH(4) =" << std::dec << static_cast<uint32_t>(bitfield.CH) << " (Chain bit)\n";
    std::cout << "  ; IOC(5)=" << std::dec << static_cast<uint32_t>(bitfield.IOC) << " (Interrupt On Completion)\n";
    std::cout << "  ; IDT(6)=" << std::dec << static_cast<uint32_t>(bitfield.IOC) << " (Immediate Data)\n";
    auto trb_type{static_cast<uint32_t>(bitfield.trb_type)};
    std::cout << "  ; trb_type(15:10)=" << std::dec << trb_type << " (";
    std::cout << TRB_type_name(trb_type) << ")\n";
    std::cout << "  ; DIR(16)=" << std::dec << static_cast<uint32_t>(bitfield.IOC) << " (Direction; 1 -> 'IN (Read Data)')\n";
    std::cout << "  mov eax, " << print_hex(words[3], 8) << "\n\n";
  }

  struct data_stage_trb_bf {
    uint32_t data_buffer_lo;
    uint32_t data_buffer_hi;
    struct {
      uint32_t trb_transfer_length : 17;
      uint32_t td_size : 5;
      uint32_t interrupter_target : 10;
    };
    struct {
      uint32_t C : 1; /*Cycle bit (C). This bit is used to mark the Enqueue Pointer of the Transfer ring.*/
      uint32_t ENT : 1; /*Evaluate Next TRB (ENT). If this flag is ‘1’ the xHC shall fetch and evaluate the next TRB before saving the endpoint state. Refer to section 4.12.3 for more information.*/
      uint32_t ISP : 1; /*Interrupt-on Short Packet (ISP). If this flag is ‘1’ and a Short Packet is encountered for this TRB (that is, less than the amount specified in TRB Transfer Length), then a Transfer Event TRB shall be generated with its Completion Code set to Short Packet. The TRB Transfer Length field in the Transfer Event TRB shall reflect the residual number of bytes not transferred into the associated data buffer. In either case, when a Short Packet is encountered, the TRB shall be retired without error and the xHC shall advance to the Status Stage TD.
                          if the ISP and IOC flags are both ‘1’ and a Short Packet is detected, then only one Transfer Event TRB shall be queued to the Event Ring. Also refer to section 4.10.1.1.*/
      uint32_t NS : 1; /*No Snoop (NS). When set to ‘1’, the xHC is permitted to set the No Snoop bit in the Requester Attributes of the PCIe transactions it initiates if the PCIe configuration Enable No Snoop flag is also set. When cleared to ‘0’, the xHC is not permitted to set PCIe packet No Snoop Requester Attribute. Refer to section 4.18.1 for more information.
                        NOTE: If software sets this bit, then it is responsible for maintaining cache consistency.*/
      uint32_t CH : 1; /*Chain bit (CH). Set to ‘1’ by software to associate this TRB with the next TRB on the Ring. A Data Stage TD is defined as a Data Stage TRB followed by zero or more Normal TRBs. The Chain bit is used to identify a multi-TRB Data Stage TD. The Chain bit is always ‘0’ in the last TRB of a Data Stage TD.*/
      uint32_t IOC : 1; /*Interrupt On Completion (IOC). If this bit is set to ‘1’, it specifies that when this TRB completes, the Host Controller shall notify the system of the completion by placing an Event TRB on the Event ring and asserting an interrupt to the host at the next interrupt threshold. Refer to section 4.10.4.*/
      uint32_t IDT : 1; /*Immediate Data (IDT). If this bit is set to ‘1’, it specifies that the Data Buffer Pointer field of this
                          TRB contains data, not a pointer. If IDT = ‘1’, the Length field shall contain a value between 1 and
                          8 to indicate the number of valid bytes from offset 0 in the TRB that should be used as data.
                          If the IDT flag is set in one Data Stage TRB of a TD, then it shall be the only Transfer TRB of the
                          TD. An Event Data TRB may also be included in the TD. Failure to follow this rule may result in
                          undefined xHC operation.*/
      uint32_t RsvdZ : 3;
      uint32_t trb_type : 6;
      uint32_t DIR : 1; /*Direction (DIR). This bit indicates the direction of the data transfer as defined in the Data State
                          TRB Direction column of Table 7. If cleared to ‘0’, the data stage transfer direction is OUT (Write
                          Data). If set to ‘1’, the data stage transfer direction is IN (Read Data). Refer to section 4.11.2.2 for
                          more information on the use of DIR.*/
      uint32_t RsvdZ_1 : 15;

    };
  };

  data_stage_trb_bf bitfield;
};


union tansfer_event_trb {
  uint32_t words[4];

  struct tansfer_event_trb_bf {
    uint32_t trb_pointer_lo;
    uint32_t trb_pointer_hi;
    struct {
      uint32_t trb_transfer_length : 24;
      uint32_t completion_code : 8;
    };
    struct {
      uint32_t C : 1; /*Cycle bit (C). This bit is used to mark the Enqueue Pointer of the Transfer ring.*/
      uint32_t Rsvd_0 : 1;
      uint32_t ED : 1; /*Event Data (ED). When set to ‘1’, the event was generated by an Event Data TRB and the Parameter Component (TRB Pointer field) contains a 64-bit value provided by the Event Data TRB. If cleared to ‘0’, the Parameter Component (TRB Pointer field) contains a pointer to the TRB that generated this event. Refer to section 4.11.5.2 for more information.*/
      uint32_t RsvdZ_1 : 7;
      uint32_t trb_type : 6;
      uint32_t endpoint_id : 5; /*The ID of the Endpoint that generated the event. This value is used as an index in the Device Context to select the Endpoint Context associated with this event.*/
      uint32_t RsvdZ_2 : 3;
      uint32_t slot_id : 8; /*Slot ID. The ID of the Device Slot that generated the event. This is value is used as an index in the Device Context Base Address Array to select the Device Context of the source device.*/
    };
  };

  tansfer_event_trb_bf bitfield;
};

// page 427
union normal_stage_trb {
  uint32_t words[4];

  struct normal_stage_trb_bf {
    uint32_t data_buffer_lo;
    uint32_t data_buffer_hi;
    struct {
      uint32_t trb_transfer_length : 17;
      uint32_t td_size : 5;
      uint32_t interrupter_target : 10;
    };
    struct {
      uint32_t C : 1; /*Cycle bit (C). This bit is used to mark the Enqueue Pointer of the Transfer ring.*/
      uint32_t ENT : 1; /*Evaluate Next TRB (ENT). If this flag is ‘1’ the xHC shall fetch and evaluate the next TRB before saving the endpoint state. Refer to section 4.12.3 for more information.*/
      uint32_t ISP : 1; /*Interrupt-on Short Packet (ISP). If this flag is ‘1’ and a Short Packet is encountered for this TRB (that is, less than the amount specified in TRB Transfer Length), then a Transfer Event TRB shall be generated with its Completion Code set to Short Packet. The TRB Transfer Length field in the Transfer Event TRB shall reflect the residual number of bytes not transferred into the associated data buffer. In either case, when a Short Packet is encountered, the TRB shall be retired without error and the xHC shall advance to the Status Stage TD.
                          if the ISP and IOC flags are both ‘1’ and a Short Packet is detected, then only one Transfer Event TRB shall be queued to the Event Ring. Also refer to section 4.10.1.1.*/
      uint32_t NS : 1; /*No Snoop (NS). When set to ‘1’, the xHC is permitted to set the No Snoop bit in the Requester Attributes of the PCIe transactions it initiates if the PCIe configuration Enable No Snoop flag is also set. When cleared to ‘0’, the xHC is not permitted to set PCIe packet No Snoop Requester Attribute. Refer to section 4.18.1 for more information.
                        NOTE: If software sets this bit, then it is responsible for maintaining cache consistency.*/
      uint32_t CH : 1; /*Chain bit (CH). Set to ‘1’ by software to associate this TRB with the next TRB on the Ring. A Data Stage TD is defined as a Data Stage TRB followed by zero or more Normal TRBs. The Chain bit is used to identify a multi-TRB Data Stage TD. The Chain bit is always ‘0’ in the last TRB of a Data Stage TD.*/
      uint32_t IOC : 1; /*Interrupt On Completion (IOC). If this bit is set to ‘1’, it specifies that when this TRB completes, the Host Controller shall notify the system of the completion by placing an Event TRB on the Event ring and asserting an interrupt to the host at the next interrupt threshold. Refer to section 4.10.4.*/
      uint32_t IDT : 1; /*Immediate Data (IDT). If this bit is set to ‘1’, it specifies that the Data Buffer Pointer field of this
                          TRB contains data, not a pointer. If IDT = ‘1’, the Length field shall contain a value between 1 and
                          8 to indicate the number of valid bytes from offset 0 in the TRB that should be used as data.
                          If the IDT flag is set in one Data Stage TRB of a TD, then it shall be the only Transfer TRB of the
                          TD. An Event Data TRB may also be included in the TD. Failure to follow this rule may result in
                          undefined xHC operation.*/
      uint32_t RsvdZ : 2;
      uint32_t BEI : 1; /*Block Event Interrupt (BEI). If this bit is set to ‘1’ and IOC = ‘1’, then the Transfer Event
                          generated by IOC shall not assert an interrupt to the host at the next interrupt threshold. Refer
                          to section 4.17.5.*/
      uint32_t trb_type : 6;
      uint32_t RsvdZ_1 : 16;

    };
  };

  normal_stage_trb_bf bitfield;
};

union status_stage_trb {
  uint32_t words[4];

  void print() {
    std::cout << "; status\n";
    for (uint32_t i{}; i < 4; ++i) {
      std::cout << "mov eax, 0x" << std::hex << words[i] << "\n";
    }
  }

  void print_verbose() {
    std::cout << "  ; status TRB\n";

    for (uint32_t i{}; i < 2; ++i) {
      std::cout << "  ; ************ DWORD[" << i << "] ************\n";
      std::cout << "  mov eax, " << print_hex(words[i], 8) << "\n\n";
    }

    std::cout << "  ; ************ DWORD[2] ************\n";
    auto interrupter_target{ static_cast<uint32_t>(bitfield.interrupter_target) };

    std::cout << "  ; interrupter_target(31:22)=" << print_hex(interrupter_target, 3) << std::dec << " : " << interrupter_target << "d\n";
    std::cout << "  mov eax, " << print_hex(words[2], 8) << "\n\n";
    
    std::cout << "  ; ************ DWORD[3] ************\n";
    std::cout << "  ; C(0)  =" << std::dec << static_cast<uint32_t>(bitfield.C) << " (Cycle bit)\n";
    std::cout << "  ; ENT(1)=" << std::dec << static_cast<uint32_t>(bitfield.ENT) << " (Evaluate Next TRB)\n";    
    std::cout << "  ; CH(4) =" << std::dec << static_cast<uint32_t>(bitfield.CH) << " (Chain bit)\n";
    std::cout << "  ; IOC(5)=" << std::dec << static_cast<uint32_t>(bitfield.IOC) << " (Interrupt On Completion)\n";    
    auto trb_type{ static_cast<uint32_t>(bitfield.trb_type) };
    std::cout << "  ; trb_type(15:10)=" << std::dec << trb_type << " (";
    std::cout << TRB_type_name(trb_type) << ")\n";
    std::cout << "  ; DIR(16)=" << std::dec << static_cast<uint32_t>(bitfield.IOC) << " (Direction; 1 -> 'IN (Read Data)')\n";
    std::cout << "  mov eax, " << print_hex(words[3], 8) << "\n\n";
  }

  struct status_stage_trb_bf {
    uint32_t RsvdZ_0;
    uint32_t RsvdZ_1;
    struct {
      uint32_t RsvdZ_2 : 22;
      uint32_t interrupter_target : 10; /*Interrupter Target. This field defines the index of the Interrupter that will receive events
                                          generated by this TRB. Valid values are between 0 and MaxIntrs-1.*/
    };
    struct {
      uint32_t C : 1; /*Cycle bit (C). This bit is used to mark the Enqueue Pointer of the Transfer ring.*/
      uint32_t ENT : 1; /*Evaluate Next TRB (ENT). If this flag is ‘1’ the xHC shall fetch and evaluate the next TRB before
                          saving the endpoint state. Refer to section 4.12.3 for more information.*/
      uint32_t RsvdZ_2 : 2;
      uint32_t CH : 1; /*Chain bit (CH). Set to ‘1’ by software to associate this TRB with the next TRB on the Ring. A
                          Status Stage TD is defined as a Status Stage TRB followed by zero or one Event Data TRB. The
                          Chain bit is used to identify a multi-TRB Status Stage TD. The Chain bit is always ‘0’ in the last
                          TRB of a Status Stage TD.*/
      uint32_t IOC : 1; /*Interrupt On Completion (IOC). If this bit is set to ‘1’, it specifies that when this TRB completes,
                          the Host Controller shall notify the system of the completion by placing an Event TRB on the
                          Event ring and asserting an interrupt to the host at the next interrupt threshold. Refer to section
                          4.10.4.*/
      uint32_t RsvdZ_3 : 4;
      uint32_t trb_type : 6;
      uint32_t DIR : 1; /*Direction (DIR). This bit indicates the direction of the data transfer as defined in the Status State
TRB Direction column of Table 7. If cleared to ‘0’, the status stage transfer direction is OUT
(Host-to-device). If set to ‘1’, the status stage transfer direction is IN (Device-to-host). Refer to
section 4.11.2.2 for more information on the use of DIR.*/
      uint32_t RsvdZ_4 : 15;
    };
  };

  status_stage_trb_bf bitfield;
};

union device {
  uint64_t qword[3];
  struct device_bf {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t bcdUSB;
    uint8_t bDeviceClass;
    uint8_t bDeviceSubClass;
    uint8_t bDeviceProtocol;
    uint8_t bMaxPacketSize0;
    uint16_t idVendor;
    uint16_t idProduct;
    uint16_t bcdDevice;
    uint8_t iManufacturer;
    uint8_t iProduct;
    uint8_t iSerialNumber;
    uint8_t bNumConfigurations;
    uint8_t pad[6];
  };
  device_bf bitfield;
};

int main()
{
  device dev{
    0x4020000000000112,
    0x0201020080088644,
    0x0000000000000103 };
  //ring_status ring_stat{ 0x0100840101000000ull };
  ring_status ring_stat{ 0x101800501000000ull };

  { // working descriptor read
    setup_stage_trb setup_trb{ 0x01000680 ,0x01000000 ,0x00000008 ,0x00030841 };
    setup_trb.print_verbose();

    data_stage_trb data_trb{ 0x0,0x0,0x00000100 , 0x00010C01 };
    data_trb.print_verbose();

    status_stage_trb status_trb{ 0x0, 0x0, 0x0, 0x00001013 };
    status_trb.print_verbose();

    data_stage_trb data_event_trb{ 0x0,0x0,0x00000100 , 0x00001E21 };
    data_event_trb.print_verbose();
  }
  std::cout << "\n###################################################################\n\n\n";

  { // Read string descriptor
    setup_stage_trb setup_trb_ref{ 0x01000680, 0x00080000, 0x00000008, 0x00030841 };
    setup_stage_trb setup_trb{};
    setup_trb.bitfield.bRequest = 0x6;
    setup_trb.bitfield.C = 1;
    setup_trb.bitfield.IDT = 1;
    setup_trb.bitfield.wIndex[0] = 0;
    setup_trb.bitfield.wIndex[0] = 3;
    setup_trb.bitfield.wLength = 8;
    setup_trb.bitfield.trb_transfer_length = 8;
    setup_trb.bitfield.trb_type = 2;
    setup_trb.bitfield.request_type.transfer_direction = 1;
    setup_trb.bitfield.trt = 3;

    setup_trb.print_verbose();

    data_stage_trb data_trb{ 0x0,0x0,0x8,0x00010C01 };
    data_trb.bitfield.C = 1;
    data_trb.bitfield.RsvdZ = 0;
    data_trb.bitfield.trb_transfer_length = 8;
    data_trb.bitfield.DIR = 1;
    data_trb.bitfield.CH = 0;
    data_trb.bitfield.trb_type = 3;
    data_trb.print_verbose();

    status_stage_trb status_trb{ 0x0,0x0,0x0,0x00011013 };
    status_trb.bitfield.CH = 1;
    status_trb.bitfield.ENT = 1;
    status_trb.print_verbose();

    data_stage_trb data_event_trb{ 0x0, 0x0, 0x0, 0x00001E21 };
    data_event_trb.print_verbose();
  }
  tansfer_event_trb trans_trb{ 0x0,0x0,0x04000008, 0x01038001 };

  int b = 0;
  b = 1;
  /*setup_stage_trb trb{ 0x01000680u , 0x00080000u , 0x00000008u, 0x00030841u };
  data_stage_trb trb_data{ 0x0,0x0,0x00000008,0x00010C01 };
  status_stage_trb trb_status{ 0x0,0x0,0x0,0x00001013 };*/
  // Send Set Report
  {
    setup_stage_trb trb{ 0x00010900 , 0x00000000 , 0x00000008, 0x00000841 };
    status_stage_trb trb_status{ 0x0,0x0,0x0,0x00001013 };
    data_stage_trb trb_data{ 0x0,0x0,0x0,0x00001E21 };
    int a = 0;
    a = 1;
  }

  // Send Set protocol
  {
    setup_stage_trb trb{ 0x00000B21,0x00000000, 0x00000008 ,0x00000841 };
    status_stage_trb trb_status{ 0x0,0x0,0x0,0x00001013 };
    data_stage_trb trb_data{ 0x0,0x0,0x0,0x00001E21 };
    int a = 0;
    a = 1;
  }

  {
    normal_stage_trb normal_trb{ 0x0,0x0,0x00000008 ,0x00000413 };
    data_stage_trb trb_data{ 0x0,0x0,0x00400000,0x00001C21 };
    int a = 0;
    a = 1;
  }


  completion_trb comp_trb{};
  comp_trb.words[0] = 0x1000000ull;
  comp_trb.words[1] = 0x880101000000ull;
  std::vector<uint8_t> buffer;
  std::string dump_filename{ "C:\\development\\assembly\\dump\\pcie_enum_offset.bin" };


  if (FileIO::read(dump_filename, buffer)) {


  }

}

