#ifndef GSSLOCALBUFFER_H
#define GSSLOCALBUFFER_H

#include <filesystem>
#include <vector>
#include <utility>
#include <gssapi/gssapi.h>
#include <boost/endian/arithmetic.hpp>

#include "gssapibuffer.hpp"
#include "gsstrace.hpp"

namespace gssxx {

  class GssLocalBuffer : public GssApiBuffer {
  public:
    GssLocalBuffer()
      : gssBuffer_ {0, nullptr}
      , gssBufferPtr_ {GSS_C_NO_BUFFER}
      , receiveDataLength_ {0}
    {
      trace("GssLocalBuffer()");
    }

    GssLocalBuffer(std::size_t size)
      : data_(size)
      , gssBuffer_ {size, data_.data()}
      , gssBufferPtr_ {&gssBuffer_}
      , receiveDataLength_ {static_cast<boost::endian::big_uint32_t::value_type>(size)}
    {
      trace("GssLocalBuffer(" + std::to_string(size) + ")");
    }

    GssLocalBuffer(const std::string& s)
      : data_ {s.begin(), s.end()}
      , gssBuffer_ {data_.size(), data_.data()}
      , gssBufferPtr_ {&gssBuffer_}
      , receiveDataLength_ {static_cast<boost::endian::big_uint32_t::value_type>(data_.size())}
    {
      trace("GssLocalBuffer(std::string)");
    }

    // Copy Constructor
    GssLocalBuffer(const GssLocalBuffer& other)
      : data_ {other.data_}
      , gssBuffer_ {data_.size(), data_.data()}
      , gssBufferPtr_ {&gssBuffer_}
      , receiveDataLength_ {other.receiveDataLength_}
    {
      trace("GssLocalBuffer() (copy constructor)");
    }

    // Move Constructor
    GssLocalBuffer(GssLocalBuffer&& other) noexcept
      : data_ {std::move(other.data_)}
      , gssBuffer_ {other.gssBuffer_}
      , gssBufferPtr_ {&gssBuffer_}
      , receiveDataLength_ {other.receiveDataLength_}
    {
      trace("GssLocalBuffer() (move constructor)");
      other.gssBuffer_ = {0, nullptr};
      other.gssBufferPtr_ = GSS_C_NO_BUFFER;
    }

    // Copy from another buffer
    GssLocalBuffer(const GssBuffer& other)
      : data_ {other.begin(), other.end()}
      , gssBuffer_ {data_.size(), data_.data()}
      , gssBufferPtr_ {&gssBuffer_}
      , receiveDataLength_ {static_cast<boost::endian::big_uint32_t::value_type>(data_.size())}
    {
      trace("GssLocalBuffer(GssBuffer)");
    }


    GssLocalBuffer& operator=(const GssLocalBuffer& other) = delete;

    GssLocalBuffer& operator=(GssLocalBuffer&& other) noexcept
    {
      trace("GssLocalBuffer (Move Assignment)");
      data_ = std::move(other.data_);
      gssBuffer_ = other.gssBuffer_;
      gssBufferPtr_ = &gssBuffer_;
      other.gssBuffer_ = {0, nullptr};
      other.gssBufferPtr_ = GSS_C_NO_BUFFER;
      return *this;
    }

    virtual ~GssLocalBuffer()
    {
      trace("~GssLocalBuffer");
    }

    void reset(std::size_t size)
    {
      trace("GssLocalBuffer::reset(" + std::to_string(size) + ")");
      data_ = std::vector<char>(size);
      gssBuffer_ = {size, data_.data()};
      gssBufferPtr_ = &gssBuffer_;
    }

    virtual std::size_t size() const override final
    {
      return data_.size();
    }

    virtual const void* data() const override final
    {
      return data_.data();
    }

    virtual operator gss_buffer_t() override final
    {
      return gssBufferPtr_;
    }

    operator boost::asio::mutable_buffer()
    {
      return boost::asio::mutable_buffer(data_.data(), data_.size());
    }

    void load(const std::filesystem::path& filePath);

    void receive(boost::asio::ip::tcp::socket& socket);
    void receiveAsync(boost::asio::ip::tcp::socket& socket, Handler handler);

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
  }; // class GssLocalBuffer
}  // gssxx

#endif /* GSSLOCALBUFFER_H */
