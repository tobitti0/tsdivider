#ifndef _TSSP_PAT_HPP_
#define _TSSP_PAT_HPP_

#include <cstdint>

#include "ts_reader.hpp"
#include "table_header.hpp"
#include "crc.hpp"


namespace tssp
{
class pat : public table_header<pat>
{
public:
  pat(const packet& packet) :
    packet_(packet)
  {}

  const packet& get_packet() const {
    return packet_;
  }

  uint16_t transport_stream_id() const {
    return (*(payload()+4) << 8) + *(payload()+5);
  }

  uint8_t section_number() const {
    return *(payload()+7);
  }

  uint8_t last_section_number() const {
    return *(payload()+8);
  }

  size_t program_num() const {
    return (section_length() - 9) / 4;
  }

  uint16_t program_number(size_t i) const {
    size_t ofs = 9 + i*4;
    return (*(payload()+ofs) << 8) + *(payload()+ofs+1);
  }

  uint16_t program_map_pid(size_t i) const {
    size_t ofs = 9 + i*4 + 2;
    return ((*(payload()+ofs) & 0x1F) << 8) + *(payload()+ofs+1);
  }

  uint32_t crc32() const {
    size_t ofs = 9 + program_num()*4;
    return
      (*(payload()+ofs) << 24) +
      (*(payload()+ofs+1) << 16) +
      (*(payload()+ofs+2) << 8) +
       *(payload()+ofs+3);
  }

  bool check_crc32() const {
    crc32_ts crc_calc;
    crc_calc.process_bytes(
        payload()+1, section_length()-1);
    return crc_calc() == crc32();
  }

private:
  const packet& packet_;
};

}

#endif