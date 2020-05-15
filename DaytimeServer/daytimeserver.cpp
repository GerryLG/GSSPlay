#include <iostream>
#include <boost/asio.hpp>

#include "tcp_server.hpp"

int main()
{
  try {
    boost::asio::io_context io_context;
    tcp_server server {io_context, 1313, "sample", "/home/gerry/sample.keytab"};
    io_context.run();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Some other exception!" << std::endl;
  }

  return 0;
}
