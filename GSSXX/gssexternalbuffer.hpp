#ifndef GSSEXTERNALBUFFER_H
#define GSSEXTERNALBUFFER_H

#include <iostream>

#include "gssbuffer.hpp"

namespace gssxx {

  class GssExternalBuffer : public GssBuffer {
  public:
    //! Default constructor
    GssExternalBuffer()
      : gssBuffer_ {0, nullptr}
    {
      std::cerr << "GssExternalBuffer()" << std::endl;
    }

    //! Copy constructor
    GssExternalBuffer(const GssExternalBuffer& other) = delete;

    //! Move constructor
    GssExternalBuffer(GssExternalBuffer&& other) noexcept
    {
      std::cerr << "GssExternalBuffer() (Move Constructor)" << std::endl;
      gssBuffer_ = other.gssBuffer_;
      other.gssBuffer_ = {0, nullptr};
    }

    //! Destructor
    virtual ~GssExternalBuffer() noexcept
    {
      std::cerr << "~GssExternalBuffer" << std::endl;
      clear();
    }

    //! Copy assignment operator
    GssExternalBuffer& operator=(const GssExternalBuffer& other) = delete;

    //! Move assignment operator
    GssExternalBuffer& operator=(GssExternalBuffer&& other) noexcept
    {
      std::cerr << "GssExternalBuffer (move assignment)" << std::endl;
      clear();
      gssBuffer_ = other.gssBuffer_;
      other.gssBuffer_ = {0, nullptr};

      return *this;
    }

    virtual std::size_t size() const override final
    {
      return gssBuffer_.length;
    }

    virtual operator gss_buffer_t() override final
    {
      return &gssBuffer_;
    }

    operator boost::asio::const_buffer()
    {
      return boost::asio::const_buffer {gssBuffer_.value, gssBuffer_.length};
    }

  protected:
    virtual const void* data() const override final
    {
      return gssBuffer_.value;
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
  }; // class GssExternalBuffer

} // gssxx

#endif /* GSSEXTERNALBUFFER_H */
