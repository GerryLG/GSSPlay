#include <gssapi/gssapi_krb5.h>

#include "gssservercred.hpp"
#include "gssname.hpp"
#include "gssexception.hpp"
#include "exception"

namespace gssxx {
  
  GssServerCredential::GssServerCredential(std::string serviceName)
  {
    setCredential(serviceName);
    referenceCount_ = new int{1};
  }

  GssServerCredential::GssServerCredential(std::string serviceName, std::string keytabFile)
  {
    if (krb5_gss_register_acceptor_identity(keytabFile.c_str())) {
      throw std::runtime_error("Error setting registering keytab");
    }
    setCredential(serviceName);
    referenceCount_ = new int{1};
  }

  GssServerCredential::~GssServerCredential() noexcept
  {
    OM_uint32 minorStatus;
    if (--*referenceCount_ == 0) {
      gss_release_cred(&minorStatus, &credential_);
    }
  }

  void GssServerCredential::setCredential(std::string serviceName)
  {
    OM_uint32 majorStatus, minorStatus;
    GssName service(serviceName);

    majorStatus = gss_acquire_cred(&minorStatus, service, 0,
                                   GSS_C_NULL_OID_SET, GSS_C_ACCEPT, &credential_,
                                   nullptr, nullptr);
    if (majorStatus != GSS_S_COMPLETE) {
      throw GssException("Error aquiring server credentials", majorStatus, minorStatus);
    }
  }
}  // gssxx
