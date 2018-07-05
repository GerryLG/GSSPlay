#ifndef GSSCONTEXT_H
#define GSSCONTEXT_H

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <gssapi/gssapi.h>

#include "gssservercred.hpp"
#include "gssname.hpp"
#include "gssapibuffer.hpp"
#include "gssexternalbuffer.hpp"
#include "gssstatus.hpp"

namespace gssxx {

  class GssContext {
  public:

    enum class State {
      NotEstablished,
      Established,
      ContinueNeeded,
      Error
    };

    //! Destructor
    virtual ~GssContext() noexcept
    {
      std::cerr << "~GssContext()" << std::endl;
      if (context_ != GSS_C_NO_CONTEXT) {
        OM_uint32 minorStatus;
        gss_delete_sec_context(&minorStatus, &context_, GSS_C_NO_BUFFER);
      }
    }

    GssExternalBuffer wrap(GssApiBuffer& buffer, GssStatus& status);
    GssExternalBuffer wrap(GssApiBuffer& buffer);
    GssExternalBuffer unwrap(GssApiBuffer& buffer, GssStatus& status);
    GssExternalBuffer unwrap(GssApiBuffer& buffer);

    bool established() const
    {
      return state_ == State::Established;
    }

    std::string peerName() const
    {
      return peerName_;
    }

  protected:
    GssContext()
      : state_ {State::NotEstablished}
      , context_ {GSS_C_NO_CONTEXT}
      , peerName_ {"NoPeer"}
      , returnFlags_ {0}
    {
      std::cerr << "GssContext()" << std::endl;
    }

    State state_;
    gss_ctx_id_t context_;
    GssName peerName_;
    OM_uint32 returnFlags_;
  };

}  // gssxx

#endif /* GSSCONTEXT_H */
