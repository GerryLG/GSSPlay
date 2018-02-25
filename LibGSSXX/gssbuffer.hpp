#ifndef GSSBUFFER_H
#define GSSBUFFER_H

#include <iostream>
#include <vector>
#include <functional>
#include <boost/asio.hpp>
#include <boost/endian/arithmetic.hpp>
#include <boost/system/error_code.hpp>

#include <gssapi/gssapi.h>

#include "gssxxerror.hpp"

namespace gssxx {

  class GssBuffer {
    friend std::ostream& operator<<(std::ostream& os, const GssBuffer& buffer);

  public:
    using Handler = std::function<void(const GssxxError)>;

    virtual std::size_t size() const = 0;
    virtual operator gss_buffer_t() = 0;

    void send(boost::asio::ip::tcp::socket& socket) const;
    void sendAsync(boost::asio::ip::tcp::socket& socket, Handler handler) const;

    virtual ~GssBuffer()
    {
      std::cerr << "~GssBuffer" << std::endl;
    }

  protected:
    GssBuffer()
      : handler_ {}
      , sendDataLength_ {0}
    {
    }

    virtual const void* data() const = 0;
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
