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

    GssBuffer(const GssBuffer& other) = delete;
    GssBuffer(GssBuffer&& other) = default;

    GssBuffer& operator=(const GssBuffer& other) = delete;
    GssBuffer& operator=(GssBuffer&& other) = default;
    
    virtual std::size_t size() const = 0;
    virtual operator gss_buffer_t() = 0;

    void send(boost::asio::ip::tcp::socket& socket) const;
    void sendAsync(boost::asio::ip::tcp::socket& socket, Handler handler) const;

    unsigned char charAt(std::size_t index) const
    {
      if (data() == nullptr || index >= size()) {
        throw std::out_of_range("Index greater than buffer size");
      }

      return *(static_cast<const unsigned char *>(data()) + index);
    }

    // GssPartialBuffer partialBuffer(std::size_t offset, std::size_t size);

    virtual ~GssBuffer();

  protected:
    GssBuffer();

    virtual const void* data() const = 0;
    mutable Handler handler_;

  private:
    void bufferSizeSent(boost::asio::ip::tcp::socket* socket,
                        const boost::system::error_code& error,
                        std::size_t bytes_transferred) const;
    void bufferDataSent(boost::asio::ip::tcp::socket* socket,
                        const boost::system::error_code& error,
                        std::size_t bytes_transferred) const;

    static std::vector<der::DerItem> derParseSequenceOfSequence(const GssBuffer& sosBuffer);

    mutable boost::endian::big_uint32_t sendDataLength_;
    std::unique_ptr<der::DerParser> parserPtr_;
   };

  std::ostream& operator<<(std::ostream& os, const GssBuffer& buffer);

} // gssxx

#endif /* GSSBUFFER_H */
