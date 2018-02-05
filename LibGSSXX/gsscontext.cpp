#include <iostream>

#include "gsscontext.hpp"

using namespace gssxx;

GssExternalBuffer
GssContext::wrap(GssBuffer &buffer, GssStatus &status)
{
  std::cerr << "GssContext::wrap(buffer, status)" << std::endl;
  GssExternalBuffer returnBuffer;
  OM_uint32 majorStatus, minorStatus;

  majorStatus = gss_wrap(&minorStatus, context_, 1, GSS_C_QOP_DEFAULT, buffer, nullptr, returnBuffer);
  status = {majorStatus, minorStatus};

  return returnBuffer;
}

GssExternalBuffer
GssContext::wrap(GssBuffer &buffer)
{
  std::cerr << "GssContext::wrap(buffer)" << std::endl;
  GssStatus status;

  GssExternalBuffer returnBuffer = wrap(buffer, status);
  if (! status) {
    throw GssException("Error wrapping buffer", status);
  }

  return returnBuffer;
}

GssExternalBuffer
GssContext::unwrap(GssBuffer &buffer, GssStatus &status)
{
  std::cerr << "GssContext::unwrap(buffer, status)" << std::endl;
  GssExternalBuffer returnBuffer;
  OM_uint32 majorStatus, minorStatus;

  majorStatus = gss_unwrap(&minorStatus, context_, buffer, returnBuffer, nullptr, nullptr);
  status = {majorStatus, minorStatus};

  return returnBuffer;
}

GssExternalBuffer
GssContext::unwrap(GssBuffer &buffer)
{
  std::cerr << "GssContext::unwrap(buffer)" << std::endl;
  GssStatus status;

  GssExternalBuffer returnBuffer = unwrap(buffer, status);
  if (! status) {
    throw GssException("Error unwrapping buffer", status);
  }

  return returnBuffer;
}
