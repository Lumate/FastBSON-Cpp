/*!
 * \file strings.cpp
 * \author Nathan Eloe
 * \brief string template specializations
 */

#include "../typeinfo.h"
#include "convert_utils.h"
#include <string>
#include <memory>
#include <cstring>
#include "../element.h"

namespace bson
{
  template<>
  TypeInfo default_type<std::string>()
  {
    return STRING;
  }
  
  template<>
  std::string to_string<std::string>()
  {
    return "std::string";
  }
  
  template <>
  bool Element::check_convert<std::string>() const
  {
    return m_type == STRING || m_type == JS || m_type == DEPRECATED;
  }
  
  template<>
  unsigned Element::deserialize_bytes<std::string>(const char* bytes)
  {
    int32_t size;
    memcpy(&size, bytes, 4);
    m_data = make_void_shared(std::string(bytes + 4, size));
    return size + 4;
  }
  
  template<>
  void Element::serialize_bson<std::string>(std::ostringstream& oss) const
  {
    _to_stream(oss, *(std::static_pointer_cast<std::string>(m_data)));
    return;
  }
  
  template <>
  std::string Element::_to_std_str<std::string>() const
  {
    return *(std::static_pointer_cast<std::string>(m_data));
  }
}