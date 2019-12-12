#ifndef GSSBUFFER_H
#define GSSBUFFER_H

#include <iostream>
#include <vector>
#include <functional>
#include <stdexcept>
#include <boost/asio.hpp>
#include <boost/endian/arithmetic.hpp>
#include <boost/system/error_code.hpp>
#include <gssapi/gssapi.h>

#include "gssxxerror.hpp"
// #include "gssder.hpp"

namespace gssxx {

  // forward declarations
  class GssPartialBuffer;
  
  namespace der {
    class DerParser;
    struct DerItem;
  }
  
  class GssBuffer {
    friend std::ostream& operator<<(std::ostream& os, const GssBuffer& buffer);
    friend class GssPartialBuffer;
  public:
    using Handler = std::function<void(const GssxxError)>;
    using const_iterator = const unsigned char*;
    
    virtual std::size_t size() const = 0;
    virtual const void* data() const = 0;

    void send(boost::asio::ip::tcp::socket& socket) const;
    void sendAsync(boost::asio::ip::tcp::socket& socket, Handler handler) const;

    unsigned char charAt(std::size_t index) const
    {
      if (data() == nullptr || index >= size()) {
        throw std::out_of_range("Index greater than buffer size");
      }

      return *(static_cast<const unsigned char *>(data()) + index);
    }

    const_iterator begin() const
    {
      return static_cast<const_iterator>(data());
    }

    const_iterator end() const
    {
      return begin() + size();
    }

    virtual ~GssBuffer();

  protected:
    GssBuffer();
    GssBuffer(const GssBuffer& other) : handler_ {nullptr} {}
    GssBuffer(GssBuffer&& other) = default;
    
    GssBuffer& operator=(const GssBuffer& other)
    {
      handler_ = nullptr;
      return *this;
    }

    GssBuffer& operator=(GssBuffer&& other) = default;

    mutable Handler handler_;

  private:
    void bufferSizeSent(boost::asio::ip::tcp::socket* socket,
                        const boost::system::error_code& error,
                        std::size_t bytes_transferred) const;
    void bufferDataSent(boost::asio::ip::tcp::socket* socket,
                        const boost::system::error_code& error,
                        std::size_t bytes_transferred) const;

    mutable boost::endian::big_uint32_t sendDataLength_;
   };

  std::ostream& operator<<(std::ostream& os, const GssBuffer& buffer);

} // gssxx

#endif /* GSSBUFFER_H */
