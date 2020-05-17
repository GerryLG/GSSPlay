#include "tcp_server.hpp"

void tcp_server::start_accept()
{
  std::cerr << "tcp_server::start_accept()" << std::endl;
  tcp_connection::pointer new_connection =
    tcp_connection::create(acceptor_.get_executor(), credential_);

  acceptor_.async_accept(new_connection->socket(),
                         std::bind(&tcp_server::handle_accept, this, new_connection,
                                   std::placeholders::_1));
}
  
void tcp_server::handle_accept(tcp_connection::pointer new_connection,
                   const boost::system::error_code& error)
{
  std::cerr << "tcp_server::handle_accept()" << std::endl;
  if (!error) {
    new_connection->start();
  }

  start_accept();
}
