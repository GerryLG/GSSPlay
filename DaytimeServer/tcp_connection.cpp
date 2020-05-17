#include <iostream>
#include <string>
#include <ctime>
#include <functional>

#include "tcp_connection.hpp"
#include "gssvectorbuffer.hpp"
#include "gssexternalbuffer.hpp"

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

  std::unique_ptr<GssVectorBuffer> buffer {new GssVectorBuffer {make_daytime_string()}};
  auto wrappedBuffer = new GssExternalBuffer;
  *wrappedBuffer = context_.wrap(*buffer);
  message_.reset(wrappedBuffer);

  message_->sendAsync(socket_, std::bind(&tcp_connection::handle_write,
                                         shared_from_this(),
                                         std::placeholders::_1));
}

void
tcp_connection::handle_write(const GssxxError& error)
{
  std::cerr << "tcp_connection::handle_write()" << std::endl;

  try {
    auto authDataVec = context_.getAuthData();
    std::cerr << "Found " << authDataVec.size() << " Authdata sections." << std::endl;
    for (auto& authData : authDataVec) {
      std::cout << "AuthDataType: " << authData.authdataType() << std::endl;
      std::cout << authData.data();
    }
  } catch (GssException e) {
    std::cerr << e.what() << std::endl;
    std::cerr << e.message() << std::endl;
  }

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
