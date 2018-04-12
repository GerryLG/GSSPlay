#ifndef GSSCLIENTCONTEXT_H
#define GSSCLIENTCONTEXT_H

#include <boost/asio.hpp>
#include <gssapi/gssapi.h>

#include "gssbuffer.hpp"
#include "gssname.hpp"
#include "gsscontext.hpp"

namespace gssxx {

  class GssClientContext : public GssContext {
  public:

    GssClientContext()
    {
      std::cerr << "GssClientContext()" << std::endl;
    }

    void InitiateContext(boost::asio::ip::tcp::socket& socket, const std::string& peerName);
  };

}  // gssxx

#endif /* GSSCLIENTCONTEXT_H */
