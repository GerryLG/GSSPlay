#ifndef DAYTIMESERVER_H
#define DAYTIMESERVER_H

#include <iostream>
#include <functional>
#include <boost/asio.hpp>

#include "tcp_connection.hpp"
#include "gssservercred.hpp"

class tcp_server
{
  using tcp = boost::asio::ip::tcp;
public:
  tcp_server(boost::asio::io_service& io_service)
    : acceptor_(io_service, tcp::endpoint(tcp::v4(), 1313))
    , credential_{"sample", "/home/gerry/sample.keytab"}
  {
    start_accept();
  }
private:
  void start_accept();
  void handle_accept(tcp_connection::pointer new_connection,
		     const boost::system::error_code& error);
  
  tcp::acceptor acceptor_;
  gssxx::GssServerCredential credential_;
};

#endif /* DAYTIMESERVER_H */