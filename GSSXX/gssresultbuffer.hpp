#ifndef GSSRESULTBUFFER_H
#define GSSRESULTBUFFER_H

#include <iostream>

#include "gssapibuffer.hpp"
#include "gsstrace.hpp"

namespace gssxx {

  class GssResultBuffer : public GssApiBuffer {
  public:
    //! Default constructor
    GssResultBuffer()
      : gssBuffer_ {0, nullptr}
    {
      trace("GssResultBuffer()");
    }

    //! Copy constructor
    GssResultBuffer(const GssResultBuffer& other) = delete;

    //! Move constructor
    GssResultBuffer(GssResultBuffer&& other) noexcept
    {
      trace("GssResultBuffer() (Move Constructor)");
      gssBuffer_ = other.gssBuffer_;
      other.gssBuffer_ = {0, nullptr};
    }

    //! Destructor
    virtual ~GssResultBuffer() noexcept
    {
      trace("~GssResultBuffer");
      clear();
    }

    //! Copy assignment operator
    GssResultBuffer& operator=(const GssResultBuffer& other) = delete;

    //! Move assignment operator
    GssResultBuffer& operator=(GssResultBuffer&& other) noexcept
    {
      trace("GssResultBuffer (move assignment)");
      clear();
      gssBuffer_ = other.gssBuffer_;
      other.gssBuffer_ = {0, nullptr};

      return *this;
    }

    virtual std::size_t size() const override final
    {
      return gssBuffer_.length;
    }

    virtual const void* data() const override final
    {
      return gssBuffer_.value;
    }

    virtual operator gss_buffer_t() override final
    {
      return &gssBuffer_;
    }

    operator boost::asio::const_buffer()
    {
      return boost::asio::const_buffer {gssBuffer_.value, gssBuffer_.length};
    }

  private:
    void clear()
    {
      if (gssBuffer_.value != nullptr) {
        OM_uint32 minorStatus;
        gss_release_buffer(&minorStatus, &gssBuffer_);
      }
    }

    gss_buffer_desc gssBuffer_;
  }; // class GssResultBuffer

} // gssxx

#endif /* GSSRESULTBUFFER_H */
