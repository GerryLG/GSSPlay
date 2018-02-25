#ifndef GSSSTATUS_H
#define GSSSTATUS_H

#include <string>
#include <vector>

#include <gssapi/gssapi.h>

namespace gssxx {

  class GssStatus {
  public:
    GssStatus()
      : majorStatus_ {GSS_S_COMPLETE}
      , minorStatus_ {0}
    {
    }

    GssStatus(OM_uint32 majorStatus, OM_uint32 minorStatus)
      : majorStatus_ {majorStatus}
      , minorStatus_ {minorStatus}
    {
    }

    std::string message() const;
    std::vector<std::string> majorMessage() const;
    std::vector<std::string> minorMessage() const;

    OM_uint32 getMajorStatus() const
    {
      return majorStatus_;
    }

    OM_uint32 getMinorStatus() const
    {
      return minorStatus_;
    }

    operator bool() const
    {
      return majorStatus_ == GSS_S_COMPLETE;
    }

  private:
    OM_uint32 majorStatus_;
    OM_uint32 minorStatus_;
  };

}  // gssxx

#endif /* GSSSTATUS_H */
