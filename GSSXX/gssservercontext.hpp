#ifndef GSSSERVERCONTEXT_H
#define GSSSERVERCONTEXT_H

#include <iostream>
#include <boost/asio.hpp>
#include <boost/endian/arithmetic.hpp>
#include <gssapi/gssapi.h>
#include <string>
#include <memory>
#include <vector>

#include "gssapibuffer.hpp"
#include "gssname.hpp"
#include "gsspac.hpp"
#include "gssservercred.hpp"
#include "gssxxerror.hpp"
#include "gsscontext.hpp"

namespace gssxx {

  class GssServerContext : public GssContext {
  public:
    using Callback = std::function<void(const GssxxError&)>;

    GssServerContext(const GssServerCredential& serverCred)
      : socketPtr_ {nullptr}
      , credential_ {serverCred}
      , callback_ {nullptr}
    {
      std::cerr << "GssServerContext()" << std::endl;
    }

    GssServerContext(const GssServerContext&) = delete; // No copying

    void acceptContextAsync(boost::asio::ip::tcp::socket& socket, Callback callback);

    const GssPac& Pac() const { return pac_; }

  private:
    void receivedToken(std::shared_ptr<GssApiBuffer> buffer,
                       GssxxError error);
    void sentToken(std::shared_ptr<GssApiBuffer> buffer,
                   GssxxError error);
    void contextComplete(const GssxxError& status);

    boost::asio::ip::tcp::socket* socketPtr_;
    GssServerCredential credential_;
    GssPac pac_;

    Callback callback_;
  };

} // gssxx

#endif /* GSSSERVERCONTEXT_H */
