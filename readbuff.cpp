#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>

#include "gsslocalbuffer.hpp"
#include "gsspac.hpp"

using namespace std::literals;

int main(int argc, char *argv[])
{
  std::cout << __cplusplus << std::endl;

  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <infilename>" << std::endl;
    std::exit(1);
  }

  gssxx::GssLocalBuffer buffer;
  buffer.load(argv[1]);

  gssxx::GssPac pac(buffer);

  std::cout << "PAC Account Name:  " << pac.AccountName()        << std::endl;
  std::cout << "PAC Full Name:     " << pac.FullName()           << std::endl;
  std::cout << "PAC Logon Domain:  " << pac.LogonDomain()        << std::endl;
  std::cout << "PAC Logon Server:  " << pac.LogonServer()        << std::endl;
  std::cout << "PAC Logon Time:    "
            << std::put_time(std::gmtime(&pac.LogonTime()), "%c %Z")
            << std::endl;
  std::cout << "PAC Passwd Change: "
            << std::put_time(std::gmtime(&pac.LastPasswordChange()), "%c %Z")
            << std::endl;
  std::cout << "PAC Dns Domain:    " << pac.DnsDomainName()      << std::endl;
  std::cout << "PAC UPN:           " << pac.UpnName()            << std::endl;

  std::cout << "User SID:          " << pac.UserSid()            << std::endl;
  std::cout << "Group SIDs:" << std::endl;
  for (const auto& sid : pac.groupSids()) {
    std::cout << "  " << sid << std::endl;
  }

  std::cout << "Finished" << std::endl;
}
