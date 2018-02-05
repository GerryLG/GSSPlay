#include <iostream>
#include <boost/asio.hpp>

#include "tcp_server.hpp"

int main()
{
  try {
    boost::asio::io_service io_service;
    tcp_server server(io_service);
    io_service.run();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Some other exception!" << std::endl;
  }

  return 0;
}
