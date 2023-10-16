#include <iostream>

#include "gsscontext.hpp"

using namespace gssxx;

GssResultBuffer
GssContext::wrap(GssApiBuffer& buffer, GssStatus& status)
{
  trace("GssContext::wrap(buffer, status)");
  GssResultBuffer returnBuffer;
  OM_uint32 majorStatus, minorStatus;

  majorStatus = gss_wrap(&minorStatus, context_, 1, GSS_C_QOP_DEFAULT, buffer, nullptr, returnBuffer);
  status = {majorStatus, minorStatus};

  return returnBuffer;
}

GssResultBuffer
GssContext::wrap(GssApiBuffer& buffer)
{
  trace("GssContext::wrap(buffer)");
  GssStatus status;

  GssResultBuffer returnBuffer = wrap(buffer, status);
  if (! status) {
    throw GssException("Error wrapping buffer", status);
  }

  return returnBuffer;
}

GssResultBuffer
GssContext::unwrap(GssApiBuffer& buffer, GssStatus& status)
{
  trace("GssContext::unwrap(buffer, status)");
  GssResultBuffer returnBuffer;
  OM_uint32 majorStatus, minorStatus;

  majorStatus = gss_unwrap(&minorStatus, context_, buffer, returnBuffer, nullptr, nullptr);
  status = {majorStatus, minorStatus};

  return returnBuffer;
}

GssResultBuffer
GssContext::unwrap(GssApiBuffer& buffer)
{
  trace("GssContext::unwrap(buffer)");
  GssStatus status;

  GssResultBuffer returnBuffer = unwrap(buffer, status);
  if (! status) {
    throw GssException("Error unwrapping buffer", status);
  }

  return returnBuffer;
}
