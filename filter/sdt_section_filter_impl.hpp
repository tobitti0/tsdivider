#ifndef _TSSP_SDT_SECTION_FILTER_IMPL_HPP_
#define _TSSP_SDT_SECTION_FILTER_IMPL_HPP_

#include "context.hpp"
#include "sdt.hpp"

namespace tssp
{

void sdt_section_filter::do_handle_section(
    context& c,
    const char* section_buffer,
    size_t section_length) {
  service_description_table sdt;
  sdt.unpack(section_buffer, section_length);

  c.get_view().print(
      c.get_packet_num(),
      sdt,
      last_version_ != sdt.header.version);

  last_version_ = sdt.header.version;
}

}

#endif
