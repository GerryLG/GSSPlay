#ifndef GSSAUTHDATA_H
#define GSSAUTHDATA_H

#include <string>
#include <vector>

#include "gssexternalbuffer.hpp"
#include "gssvectorbuffer.hpp"
#include "gsspartialbuffer.hpp"

namespace gssxx {

  class GssAuthData {
  public:
    static std::vector<GssAuthData> Parse(const GssBuffer& buffer);

    uint32_t authdataType()
    {
      return authdataType_;
    }

    GssVectorBuffer& data()
    {
      return data_;
    }

  private:
    GssAuthData(uint32_t type, const GssBuffer& source)
      : authdataType_ {type}
      , data_ {source}
    {}

    uint32_t authdataType_;
    GssVectorBuffer data_;
 };

}  // gssxx

#endif /* GSSAUTHDATA_H */
