#include "gsslocalbuffer.hpp"

using namespace gssxx;
using namespace boost;
using namespace boost::endian;

using tcp = boost::asio::ip::tcp;

void
GssVectorBuffer::receive(tcp::socket& socket)
{
  std::cerr << "GssVectorBuffer::receive()" << std::endl;
  big_uint32_t bufferSize;

  asio::read(socket, asio::buffer(&bufferSize, sizeof(bufferSize)));
  reset(bufferSize);
  asio::read(socket, asio::buffer(*this));
}

void
GssVectorBuffer::receiveAsync(tcp::socket &socket, Handler handler)
{
  std::cerr << "GssVectorBuffer::receiveAsync" << std::endl;
  handler_ = handler;

  asio::async_read(socket,
                   asio::buffer(&receiveDataLength_, sizeof(receiveDataLength_)),
                   std::bind(&GssVectorBuffer::bufferSizeReceived, this, &socket,
                             std::placeholders::_1, std::placeholders::_2));
}

void
GssVectorBuffer::bufferSizeReceived(tcp::socket* socket,
                                    const system::error_code& error,
                                    std::size_t bytesReceived)
{
  std::cerr << "GssVectorBuffer::bufferSizeReceived" << std::endl;
  if (error) {
    auto executor = socket->get_executor();
    asio::post(executor, std::bind(handler_, GssxxError {"Error receiving buffer size", error}));
    handler_ = nullptr;
    return;
  }

  reset(receiveDataLength_);
  asio::async_read(*socket, asio::buffer(data_.data(), data_.size()),
                   std::bind(&GssVectorBuffer::bufferDataReceived, this, socket,
                             std::placeholders::_1, std::placeholders::_2));
}

void
GssVectorBuffer::bufferDataReceived(tcp::socket* socket,
                                    const system::error_code& error,
                                    std::size_t bytesReceived)
{
  std::cerr << "GssVectorBuffer::bufferDataReceived" << std::endl;
  auto executor = socket->get_executor();;

  if (error) {
    asio::post(executor, std::bind(handler_, GssxxError {"Error receiving buffer data", error}));
  } else {
    asio::post(executor, std::bind(handler_, GssxxError {}));
  }

  handler_ = nullptr;
}
