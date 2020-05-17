#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include <memory>
#include <boost/asio.hpp>

#include "gssservercontext.hpp"
#include "gssservercred.hpp"
#include "gssbuffer.hpp"

class tcp_connection : public std::enable_shared_from_this<tcp_connection>
{
public:
  using tcp = boost::asio::ip::tcp;
  using pointer = std::shared_ptr<tcp_connection>;

  static pointer create(const boost::asio::ip::tcp::socket::executor_type& executor,
                        const gssxx::GssServerCredential& credential);

  tcp::socket& socket()
  {
    return socket_;
  }

  void start();

  ~tcp_connection();
  
private:
#if BOOST_ASIO_VERSION >= 101400
  tcp_connection(const boost::asio::ip::tcp::socket::executor_type& executor,
                 const gssxx::GssServerCredential& credential)
    : socket_ {executor}
    , context_ {credential}
  {
  }
#else
  tcp_connection(const boost::asio::ip::tcp::socket::executor_type& executor,
                 const gssxx::GssServerCredential& credential)
    : socket_ {executor.context()}
    , context_ {credential}
  {
  }
#endif

  void handle_write(const gssxx::GssxxError& error);

  void handle_auth(const gssxx::GssxxError& error);
  
  tcp::socket socket_;
  std::unique_ptr<gssxx::GssBuffer> message_;

  gssxx::GssServerContext context_;
};

#endif /* TCP_CONNECTION_H */
