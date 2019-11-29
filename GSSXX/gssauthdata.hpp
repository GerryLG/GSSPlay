#ifndef GSSAUTHDATA_H
#define GSSAUTHDATA_H

#include "gssexternalbuffer.hpp"

namespace gssxx {

  class GssAuthData {
  public:
    GssAuthData(GssExternalBuffer&& buffer)
      : buffer_ {std::move(buffer)}
    {
    }

    operator GssExternalBuffer&()
    {
      return buffer_;
    }

  private:
    GssExternalBuffer buffer_;
  };

}  // gssxx

#endif /* GSSAUTHDATA_H */
