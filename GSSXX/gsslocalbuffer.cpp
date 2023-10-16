#include "gsslocalbuffer.hpp"
#include <filesystem>
#include <fstream>
#include <ios>
#include <iterator>
#include <stdexcept>
#include <vector>

using namespace gssxx;
using namespace boost;
using namespace boost::endian;

using tcp = boost::asio::ip::tcp;
namespace fs = std::filesystem;

void
GssLocalBuffer::load(const fs::path& filePath)
{
  trace("GssLocalBuffer::load()");

  if ( ! fs::is_regular_file(filePath) ) {
    throw std::runtime_error("File " + std::string(filePath) + " does not exist");
  }

  std::ifstream file(filePath, std::ios::binary);
  if ( ! file ) {
    throw std::runtime_error("Unable to open file " + std::string(filePath));
  }

  std::istreambuf_iterator<char> fileIter(file), fileEnd;
  data_.reserve(fs::file_size(filePath));
  data_.assign(fileIter, fileEnd);

  gssBuffer_ = {data_.size(), data_.data()};
  gssBufferPtr_ = &gssBuffer_;
}

void
GssLocalBuffer::receive(tcp::socket& socket)
{
  trace("GssLocalBuffer::receive()");
  big_uint32_t bufferSize;

  asio::read(socket, asio::buffer(&bufferSize, sizeof(bufferSize)));
  reset(bufferSize);
  asio::read(socket, asio::buffer(*this));
}

void
GssLocalBuffer::receiveAsync(tcp::socket &socket, Handler handler)
{
  trace("GssLocalBuffer::receiveAsync");
  handler_ = handler;

  asio::async_read(socket,
                   asio::buffer(&receiveDataLength_, sizeof(receiveDataLength_)),
                   std::bind(&GssLocalBuffer::bufferSizeReceived, this, &socket,
                             std::placeholders::_1, std::placeholders::_2));
}

void
GssLocalBuffer::bufferSizeReceived(tcp::socket* socket,
                                    const system::error_code& error,
                                    std::size_t bytesReceived)
{
  trace("GssLocalBuffer::bufferSizeReceived");
  if (error) {
    auto executor = socket->get_executor();
    asio::post(executor, std::bind(handler_, GssxxError {"Error receiving buffer size", error}));
    handler_ = nullptr;
    return;
  }

  reset(receiveDataLength_);
  asio::async_read(*socket, asio::buffer(data_.data(), data_.size()),
                   std::bind(&GssLocalBuffer::bufferDataReceived, this, socket,
                             std::placeholders::_1, std::placeholders::_2));
}

void
GssLocalBuffer::bufferDataReceived(tcp::socket* socket,
                                    const system::error_code& error,
                                    std::size_t bytesReceived)
{
  trace("GssLocalBuffer::bufferDataReceived");
  auto executor = socket->get_executor();;

  if (error) {
    asio::post(executor, std::bind(handler_, GssxxError {"Error receiving buffer data", error}));
  } else {
    asio::post(executor, std::bind(handler_, GssxxError {}));
  }

  handler_ = nullptr;
}
