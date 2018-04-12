#ifndef GSSEXCEPTION_H
#define GSSEXCEPTION_H

#include <stdexcept>
#include <string>

#include <gssapi/gssapi.h>

#include "gssstatus.hpp"

namespace gssxx {

  class GssException : public std::runtime_error, public GssStatus {
  public:
    GssException(const std::string& what, OM_uint32 majorStatus, OM_uint32 minorStatus)
      : std::runtime_error {what}
      , GssStatus {majorStatus, minorStatus}
    {
    }

    GssException(const std::string& what, const GssStatus& status)
      : std::runtime_error {what}
      , GssStatus {status}
    {
    }
  };

} // gssxx

#endif /* GSSEXCEPTION_H */
