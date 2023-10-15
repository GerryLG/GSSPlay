#include <iostream>
#include <memory>
#include <string>
#include <ctime>
#include <functional>

#include "tcp_connection.hpp"
#include "gsslocalbuffer.hpp"
#include "gssresultbuffer.hpp"

using namespace gssxx;

inline
std::string
make_daytime_string()
{
  std::time_t now = std::time(0);
  return std::ctime(&now);
}

tcp_connection::~tcp_connection()
{
  std::cerr << "tcp_connection going..." << std::endl;
}

tcp_connection::pointer
tcp_connection::create(const boost::asio::ip::tcp::socket::executor_type& executor,
                       const GssServerCredential& credential)
{
  return pointer(new tcp_connection {executor, credential});
}

void
tcp_connection::start()
{
  std::cerr << "tcp_connection::start()" << std::endl;

  if (! context_.established()) {
    std::cerr << "tcp_connection: Context not yet established." <<std::endl;
    context_.acceptContextAsync(socket_,
                                std::bind(&tcp_connection::authentication_complete,
                                          shared_from_this(),
                                          std::placeholders::_1));
    return;
  }

  // Client is now authenticated so send message
  auto bufferPtr {std::make_unique<GssLocalBuffer>(make_daytime_string())};
  auto wrappedBufferPtr {std::make_unique<GssResultBuffer>()};

  *wrappedBufferPtr = context_.wrap(*bufferPtr);
  message_ = std::move(wrappedBufferPtr);
  message_->sendAsync(socket_, std::bind(&tcp_connection::message_sent,
                                         shared_from_this(),
                                         std::placeholders::_1));

  auto& pac = context_.Pac();
  if (pac.Valid()) {
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
  }
}

void
tcp_connection::message_sent(const GssxxError& error)
{
  std::cerr << "tcp_connection::message_sent()" << std::endl;
}

void
tcp_connection::authentication_complete(const GssxxError& error)
{
  std::cerr << "tcp_connection::authentication_complete()" << std::endl;

  if (error) {
    std::cerr << error.message() << std::endl;

    switch (error.errorType()) {
    case GssxxError::ErrorType::GssError:
      std::cerr << error.gssStatus().message() << std::endl;
      break;
    case GssxxError::ErrorType::SystemError:
      std::cerr << error.systemError().message() << std::endl;
      break;
    default:
      // Shouldn't really happen
      break;
    }

    return;
  }

  std::cerr << "Authenticated: " << context_.peerName() << std::endl;

  // Now client is authenticated, call start again
  start();
}
