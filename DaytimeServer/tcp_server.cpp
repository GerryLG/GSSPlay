#include "tcp_server.hpp"
#include "gsstrace.hpp"

void tcp_server::start_accept()
{
  gssxx::trace("tcp_server::start_accept()");
  tcp_connection::pointer new_connection =
    tcp_connection::create(acceptor_.get_executor(), credential_);

  acceptor_.async_accept(new_connection->socket(),
                         std::bind(&tcp_server::handle_accept, this, new_connection,
                                   std::placeholders::_1));
}
  
void tcp_server::handle_accept(tcp_connection::pointer new_connection,
                   const boost::system::error_code& error)
{
  gssxx::trace("tcp_server::handle_accept()");
  if (!error) {
    new_connection->start();
  }

  start_accept();
}
