#ifndef GSSAPIBUFFER_H
#define GSSAPIBUFFER_H

#include "gssbuffer.hpp"

namespace gssxx {

  class GssApiBuffer : public GssBuffer {
  public:
    virtual operator gss_buffer_t() = 0;
  };

}  // gssxx

#endif /* GSSAPIBUFFER_H */
