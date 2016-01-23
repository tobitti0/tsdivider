#ifndef _TSSP_SDT_SECTION_FILTER_IMPL_HPP_
#define _TSSP_SDT_SECTION_FILTER_IMPL_HPP_

#include "context.hpp"
#include "section.hpp"
#include "sdt.hpp"

namespace tssp
{

void sdt_section_filter::do_handle_section(
    context& c,
    const char* section_buffer,
    size_t section_length) {
  section s;
  s.unpack(section_buffer, section_length);
  service_description_table sdt;
  s.convert(sdt);

  bool changed = is_changed(s.header, sdt);

  c.get_view().print(
      c.get_packet_num(),
      s.header,
      sdt,
      changed);

  if(changed) {
    if(s.header.table_id == 0x42) {
      c.latest_service_descriptors.clear();
      for(auto& service : sdt.services) {
        for(auto& d : service.descriptors) {
          c.latest_service_descriptors.emplace(
              service.service_id,
              d.as<service_descriptor>());
        }
      }
    }

    version_[
      std::make_tuple(
          s.header.table_id,
          s.header.table_id_extension,
          sdt.original_network_id)] = s.header.version;
  }
}

}

#endif
