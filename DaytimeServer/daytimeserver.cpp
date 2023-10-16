#include <charconv>
#include <cstdlib>
#include <iostream>
#include <string_view>
#include <system_error>

#include <boost/asio.hpp>

#include "tcp_server.hpp"
#include "gsstrace.hpp"

void usage() {
  std::cerr << "Usage: [-t] [-p portnum] <keytab_file> <principal>" << std::endl;
  std::exit(1);
}

int main(int argc, char *argv[])
{
  unsigned short portnum = 1313;

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
      std::string_view portstr {argv[i]};
      auto [ptr, ec] = std::from_chars(portstr.cbegin(), portstr.cend(), portnum);
      if (ec != std::errc() || ptr != portstr.cend())
        usage();
      continue;
    }

    usage();
  }

  if (argc - i != 2) {
    usage();
  }

  auto keytabFilename = argv[i];
  auto principalName = argv[++i];

  try {
    boost::asio::io_context io_context;
    tcp_server server {io_context, portnum, principalName, keytabFilename};
    io_context.run();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Some other exception!" << std::endl;
  }

  return 0;
}
