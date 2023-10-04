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
                                std::bind(&tcp_connection::handle_auth,
                                          shared_from_this(),
                                          std::placeholders::_1));
    return;
  }

  auto bufferPtr {std::make_unique<GssVectorBuffer>(make_daytime_string())};
  auto wrappedBufferPtr {std::make_unique<GssExternalBuffer>()};

  *wrappedBufferPtr = context_.wrap(*bufferPtr);
  message_ = std::move(wrappedBufferPtr);
  message_->sendAsync(socket_, std::bind(&tcp_connection::handle_write,
                                         shared_from_this(),
                                         std::placeholders::_1));
}

void
tcp_connection::handle_write(const GssxxError& error)
{
  std::cerr << "tcp_connection::handle_write()" << std::endl;

  // TODO: This is where we did gathering the auth data.
}

void
tcp_connection::handle_auth(const GssxxError& error)
{
  std::cerr << "tcp_connection::handle_auth()" << std::endl;

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
  start();
}
