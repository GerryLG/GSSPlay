#include <array>
#include <charconv>
#include <cstdlib>
#include <iostream>

#include <boost/asio.hpp>

#include "gssclientcontext.hpp"
#include "gssexception.hpp"
#include "gsslocalbuffer.hpp"
#include "gsstrace.hpp"

using boost::asio::ip::tcp;


void usage() {
  std::cerr << "Usage: [-t] [-p portnum] <hostname> <principal>" << std::endl;
  std::exit(1);
}

int main(int argc, char* argv[])
{
  std::string portnum {"1313"};

  int i;
  for (i = 1; i < argc; ++i) {
    std::string_view arg {argv[i]};
    if (arg.empty() || arg.front() != '-')
      break;

    if (arg == "-t") {
      gssxx::setTrace(true);
      continue;
    }

    if (arg == "-p") {
      if (++i == argc)
        usage();
      portnum = argv[i];
      continue;
    }

    usage();
  }

  if (argc - i != 2) {
    usage();
  }

  auto hostName = std::string(argv[i]);
  auto principalName = std::string(argv[++i]);


  try {
    boost::asio::io_context io_context;

    gssxx::GssClientContext context;

    tcp::resolver resolver {io_context};
    tcp::resolver::results_type endpoint_iterator = resolver.resolve(hostName, portnum);

    tcp::socket socket {io_context};
    boost::asio::connect(socket, endpoint_iterator);

    try {
      context.InitiateContext(socket, principalName + "@" + hostName);
    } catch (gssxx::GssException e) {
      std::cerr << "GssException: " << e.what() << std::endl;
      std::cerr << e.message() << std::endl;
      return 1;
    }

    gssxx::GssLocalBuffer buffer;
    buffer.receive(socket);
    gssxx::trace(buffer);

    auto unwrappedBuffer = context.unwrap(buffer);
    gssxx::trace(unwrappedBuffer);

    std::string message(unwrappedBuffer.begin(), unwrappedBuffer.end());
    std::cout << "Recieved Message: " << message << std::endl;

  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
