#include <iostream>
#include <array>

#include <boost/asio.hpp>
#include "gssclientcontext.hpp"
#include "gssvectorbuffer.hpp"
#include "gssexception.hpp"

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
  try {
    if (argc != 2) {
      std::cerr << "Usage: client <host>" << std::endl;
      return 1;
    }

    boost::asio::io_service ioService;

    gssxx::GssClientContext context;
    
    tcp::resolver resolver(ioService);
    tcp::resolver::query query(argv[1], "1313");
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    tcp::socket socket(ioService);
    boost::asio::connect(socket, endpoint_iterator);

    try {
      context.InitiateContext(socket, std::string("sample@") + argv[1]);
    } catch (gssxx::GssException e) {
      std::cerr << "GssException: " << e.what() << std::endl;
      std::cerr << e.message() << std::endl;
      return 1;
    }

    gssxx::GssVectorBuffer buffer;
    buffer.receive(socket);
    std::cout << buffer << std::endl;

    auto unwrappedBuffer = context.unwrap(buffer);
    std::cout << unwrappedBuffer << std::endl;

  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
