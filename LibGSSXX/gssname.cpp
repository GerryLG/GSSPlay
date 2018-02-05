#include <utility>

#include "gssname.hpp"
#include "gssexception.hpp"

namespace gssxx {

  GssName::GssName(const std::string& name)
  {
    OM_uint32 majorStatus, minorStatus;
    gss_buffer_desc inName;
    inName.value = (void *)name.c_str();
    inName.length = name.size();

    majorStatus = gss_import_name(&minorStatus, &inName, GSS_C_NT_HOSTBASED_SERVICE, &name_);
    if (majorStatus != GSS_S_COMPLETE) {
      throw GssException("Error importing name " + name, majorStatus, minorStatus);
    }
  }

  GssName::GssName(const gss_name_t& name)
  {
    OM_uint32 majorStatus, minorStatus;
    majorStatus = gss_duplicate_name(&minorStatus, name, &name_);
    if (majorStatus != GSS_S_COMPLETE) {
      throw GssException("Error duplicating name", majorStatus, minorStatus);
    }
  }

  GssName::GssName(const GssName &other)
  {
    OM_uint32 majorStatus, minorStatus;
    majorStatus = gss_duplicate_name(&minorStatus, other.name_, &name_);
    if (majorStatus != GSS_S_COMPLETE) {
      throw GssException("Error duplicating name", majorStatus, minorStatus);
    }
  }

  GssName::~GssName() noexcept
  {
    if (name_) {
      OM_uint32 minorStatus;
      gss_release_name(&minorStatus, &name_);
    }
  }

  std::string GssName::to_string() const
  {
    if (! name_) {
      throw std::logic_error("Attempt to use invalid GssName");
    }

    OM_uint32 majorStatus, minorStatus;
    gss_buffer_desc outBuffer;

    majorStatus = gss_display_name(&minorStatus, name_, &outBuffer, nullptr);
    if (majorStatus != GSS_S_COMPLETE) {
      throw GssException("Error displaying name", majorStatus, minorStatus);
    }

    return std::string((char *)outBuffer.value);
  }
}  // gssxx
