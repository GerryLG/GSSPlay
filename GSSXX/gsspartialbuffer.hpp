#ifndef GSSPARTIALBUFFER_H
#define GSSPARTIALBUFFER_H

#include <stdexcept>
#include <gssapi/gssapi.h>

#include "gssbuffer.hpp"

namespace gssxx {

  class GssPartialBuffer : public GssBuffer {
  public:
    GssPartialBuffer(const GssBuffer& source, std::size_t offset, std::size_t length)
      : start_ {static_cast<const unsigned char*>(source.data()) + offset}
      , length_ {length}
    {
      if (offset + length > source.size()) {
        throw std::range_error("Partial buffer is outside the bounds of its parent.");
      }
    }

    GssPartialBuffer(const GssPartialBuffer& other) = default;
    GssPartialBuffer(GssPartialBuffer&& other) = default;

    GssPartialBuffer& operator=(const GssPartialBuffer& other) = default;
    GssPartialBuffer& operator=(GssPartialBuffer&& other) = default;

    virtual ~GssPartialBuffer() = default;

    virtual std::size_t size() const override
    {
      return length_;
    }

  protected:
    virtual const void* data() const override
    {
      return start_;
    };

  private:
    const unsigned char* start_;
    std::size_t length_;
  };

}  // gssxx

#endif /* GSSPARTIALBUFFER_H */
