#include "exception"

#include <gssapi/gssapi_krb5.h>
#include <memory>

#include "gssservercred.hpp"
#include "gssname.hpp"
#include "gssexception.hpp"

using namespace gssxx;

GssServerCredential::GssServerCredential(const std::string& serviceName)
{
  setCredential(serviceName);
  referenceCount_ = std::make_shared<int>(1);
}

GssServerCredential::GssServerCredential(const std::string& serviceName,
                                         const std::string& keytabFile)
{
  if (krb5_gss_register_acceptor_identity(keytabFile.c_str())) {
    throw std::runtime_error {"Error registering keytab"};
  }
  setCredential(serviceName);
  referenceCount_ = std::make_shared<int>(1);
}

GssServerCredential::~GssServerCredential() noexcept
{
  OM_uint32 minorStatus;
  if (--*referenceCount_ == 0) {
    gss_release_cred(&minorStatus, &credential_);
  }
}

void
GssServerCredential::setCredential(const std::string& serviceName)
{
  OM_uint32 majorStatus, minorStatus;
  GssName service {serviceName};

  majorStatus = gss_acquire_cred(&minorStatus, service, 0,
                                 GSS_C_NULL_OID_SET, GSS_C_ACCEPT, &credential_,
                                 nullptr, nullptr);
  if (majorStatus != GSS_S_COMPLETE) {
    throw GssException {"Error aquiring server credentials", majorStatus, minorStatus};
  }
}
