#include <iostream>
#include <cstdlib>

#include <boost/asio.hpp>

#include "tcp_server.hpp"

int main(int argc, char *argv[])
{
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <keytab_file>" << std::endl;
    std::exit(1);
  }

  try {
    boost::asio::io_context io_context;
    tcp_server server {io_context, 1313, "sample", argv[1]};
    io_context.run();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Some other exception!" << std::endl;
  }

  return 0;
}
