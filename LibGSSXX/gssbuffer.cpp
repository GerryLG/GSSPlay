#include "gssbuffer.hpp"
#include <boost/endian/arithmetic.hpp>
#include <boost/format.hpp>

using namespace gssxx;
using namespace boost;
using namespace boost::endian;

using tcp = boost::asio::ip::tcp;

void
GssBuffer::send(tcp::socket& socket) const
{
  std::cerr << "GssBuffer::send()" << std::endl;
  big_uint32_t bufferSize {static_cast<big_uint32_t::value_type>(size())};

  std::cerr << "BufferSize: " << bufferSize << std::endl;
  
  asio::write(socket, asio::buffer(&bufferSize, sizeof(bufferSize)));
  asio::write(socket, asio::buffer(data(), size()));
}

void
GssBuffer::sendAsync(tcp::socket &socket, Handler handler) const
{
  std::cerr << "GssBuffer::sendAsync" << std::endl;
  handler_ = handler;

  sendDataLength_ = size();
  asio::async_write(socket,
                    asio::buffer(&sendDataLength_, sizeof(sendDataLength_)),
                    std::bind(&GssBuffer::bufferSizeSent, this, &socket,
                              std::placeholders::_1, std::placeholders::_2));
}

void
GssBuffer::bufferSizeSent(tcp::socket* socket, const system::error_code& error, std::size_t bytes_transferred) const
{
  std::cerr << "GssBuffer::bufferSizeSent" << std::endl;

  if (error) {
    auto& ioService = socket->get_io_service();
    ioService.post(std::bind(handler_, GssxxError {"Error sending buffer size.", error}));
    handler_ = nullptr;
    return;
  }

  asio::async_write(*socket, asio::buffer(data(), size()),
                    std::bind(&GssBuffer::bufferDataSent, this, socket,
                              std::placeholders::_1, std::placeholders::_2));
}

void
GssBuffer::bufferDataSent(tcp::socket* socket, const system::error_code& error, std::size_t bytes_transferred) const
{
  std::cerr << "GssBuffer::bufferDataSent" << std::endl;
  auto& ioService = socket->get_io_service();
  if (error) {
    ioService.post(std::bind(handler_, GssxxError {"Error sending buffer data", error}));
  } else {
    ioService.post(std::bind(handler_, GssxxError {}));
  }
  handler_ = nullptr;
}

std::ostream& gssxx::operator<<(std::ostream& outStream, const GssBuffer& buffer)
{
  auto data = static_cast<const unsigned char*>(buffer.data());
  auto size = buffer.size();

    for (int i = 0, j = 0; i < size; ++i) {
      outStream << boost::format("%02X ") % static_cast<unsigned int>(data[i]);
      if (j == 15 || i == size - 1) {
        // end of line
        for (int k = j; k < 15; ++k) {
          outStream << "   ";
        }
        outStream << " |  ";
        for (int k = i - j; k <= i; ++k) {
          if (std::isprint(data[k]))
            outStream << data[k];
          else
            outStream << ".";
        }
        outStream << std::endl;
        j = 0;
      } else {
        ++j;
      }
    }
  
  return outStream;
}