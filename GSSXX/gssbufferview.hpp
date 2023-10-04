#ifndef GSSBUFFERVIEW_H
#define GSSBUFFERVIEW_H

#include <stdexcept>

#include "gssbuffer.hpp"

namespace gssxx {

  class GssBufferView : public GssBuffer {
  public:
    GssBufferView(const GssBuffer& source, std::size_t offset, std::size_t length)
      : start_ {static_cast<const unsigned char*>(source.data()) + offset}
      , length_ {length}
    {
      if (offset + length > source.size()) {
        throw std::range_error("BufferView is outside the bounds of its parent.");
      }
    }

    GssBufferView(const GssBufferView& other) = default;
    GssBufferView(GssBufferView&& other) = default;

    GssBufferView& operator=(const GssBufferView& other) = default;
    GssBufferView& operator=(GssBufferView&& other) = default;

    virtual ~GssBufferView() = default;

    virtual std::size_t size() const override
    {
      return length_;
    }

    virtual const void* data() const override
    {
      return start_;
    };

  private:
    const unsigned char* start_;
    std::size_t length_;
  };

}  // gssxx

#endif /* GSSBUFFERVIEW_H */
