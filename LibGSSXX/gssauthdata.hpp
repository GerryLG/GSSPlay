#ifndef GSSAUTHDATA_H
#define GSSAUTHDATA_H

#include <string>
#include <vector>

#include "gssexternalbuffer.hpp"
#include "gsspartialbuffer.hpp"

namespace gssxx {

  class GssAuthData {
  public:
    GssAuthData(GssExternalBuffer&& buffer)
      : buffer_{std::move(buffer)}
    {
    }

    operator GssExternalBuffer&()
    {
      return buffer_;
    }

    void dumpTest() const;

  private:
    GssExternalBuffer buffer_;
 };

}  // gssxx

#endif /* GSSAUTHDATA_H */
