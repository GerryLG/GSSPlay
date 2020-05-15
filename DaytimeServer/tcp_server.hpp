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
  tcp_server(boost::asio::io_context& io_context,
             unsigned short port_num,
             const std::string& service_name,
             const std::string& keytab)
    : acceptor_ {io_context, tcp::endpoint(tcp::v4(), port_num)}
    , credential_ {service_name, keytab}
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
