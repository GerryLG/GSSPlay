#ifndef GSSVECTORBUFFER_H
#define GSSVECTORBUFFER_H

#include <vector>
#include <utility>
#include <gssapi/gssapi.h>
#include <boost/endian/arithmetic.hpp>

#include "gssbuffer.hpp"

namespace gssxx {

  class GssVectorBuffer : public GssBuffer {
  public:
    GssVectorBuffer()
      : gssBuffer_ {0, nullptr}
      , gssBufferPtr_ {GSS_C_NO_BUFFER}
      , receiveDataLength_ {0}
    {
      std::cerr << "GssVectorBuffer()" << std::endl;
    }

    GssVectorBuffer(std::size_t size)
      : data_(size)
      , gssBuffer_ {size, data_.data()}
      , gssBufferPtr_ {&gssBuffer_}
      , receiveDataLength_ {static_cast<boost::endian::big_uint32_t::value_type>(size)}
    {
      std::cerr << "GssVectorBuffer(" << size << ")" << std::endl;
    }

    GssVectorBuffer(const std::string& s)
      : data_ {s.begin(), s.end()}
      , gssBuffer_ {data_.size(), data_.data()}
      , gssBufferPtr_ {&gssBuffer_}
      , receiveDataLength_ {static_cast<boost::endian::big_uint32_t::value_type>(data_.size())}
    {
      std::cerr << "GssVectorBuffer(std::string)" << std::endl;
    }

    // Copy Constructor (Deleted)
    GssVectorBuffer(const GssVectorBuffer& other) = delete;

    // Move Constructor
    GssVectorBuffer(GssVectorBuffer&& other) noexcept
      : data_ {std::move(other.data_)}
      , gssBuffer_ {other.gssBuffer_}
      , gssBufferPtr_ {&gssBuffer_}
      , receiveDataLength_ {other.receiveDataLength_}
    {
      std::cerr << "GssVectorBuffer() (move constructor)" << std::endl;
      other.gssBuffer_ = {0, nullptr};
      other.gssBufferPtr_ = GSS_C_NO_BUFFER;
    }

    GssVectorBuffer& operator=(const GssVectorBuffer& other) = delete;

    GssVectorBuffer& operator=(GssVectorBuffer&& other) noexcept
    {
      std::cerr << "GssVectorBuffer (Move Assignment)" << std::endl;
      data_ = std::move(other.data_);
      gssBuffer_ = other.gssBuffer_;
      gssBufferPtr_ = &gssBuffer_;
      other.gssBuffer_ = {0, nullptr};
      other.gssBufferPtr_ = GSS_C_NO_BUFFER;
      return *this;
    }

    virtual ~GssVectorBuffer()
    {
      std::cerr << "~GssVectorBuffer" << std::endl;
    }

    void reset(std::size_t size)
    {
      std::cerr << "GssVectorBuffer::reset(" << size << ")" << std::endl;
      data_ = std::vector<char>(size);
      gssBuffer_ = {size, data_.data()};
      gssBufferPtr_ = &gssBuffer_;
    }

    virtual std::size_t size() const override final
    {
      return data_.size();
    }

    virtual operator gss_buffer_t() override final
    {
      return gssBufferPtr_;
    }

    operator boost::asio::mutable_buffer()
    {
      return boost::asio::mutable_buffer(data_.data(), data_.size());
    }

    void receive(boost::asio::ip::tcp::socket& socket);
    void receiveAsync(boost::asio::ip::tcp::socket& socket, Handler handler);

  protected:

    virtual const void* data() const override final
    {
      return data_.data();
    }

  private:
    void bufferSizeReceived(boost::asio::ip::tcp::socket* socket,
                            const boost::system::error_code& error,
                            std::size_t bytesReceived);
    void bufferDataReceived(boost::asio::ip::tcp::socket* socket,
                            const boost::system::error_code& error,
                            std::size_t bytesReceived);

    std::vector<char> data_;
    gss_buffer_desc gssBuffer_;
    gss_buffer_t gssBufferPtr_;
    boost::endian::big_uint32_t receiveDataLength_;
  }; // class GssVectorBuffer
}  // gssxx

#endif /* GSSVECTORBUFFER_H */
