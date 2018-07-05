#include <iostream>
#include <stdexcept>
#include <functional>

#include "gssapi/gssapi_krb5.h"

#include "gssservercontext.hpp"
#include "gssexception.hpp"
#include "gssvectorbuffer.hpp"
#include "gssexternalbuffer.hpp"

using namespace gssxx;
using namespace boost;

using tcp = asio::ip::tcp;

void
GssServerContext::acceptContextAsync(tcp::socket& socket, Callback callback)
{
  std::cerr << "GssServerContext::acceptContextAsync" << std::endl;
  socketPtr_ = &socket;
  callback_ = callback;

  if (! socketPtr_->is_open()) {
    throw std::logic_error("acceptContextAsync called on a closed socket");
  }

  auto buffer = std::make_shared<GssVectorBuffer>();

  buffer->receiveAsync(socket,
                       std::bind(&GssServerContext::receivedToken,
                                 this,
                                 buffer,
                                 std::placeholders::_1));
}

void
GssServerContext::receivedToken(std::shared_ptr<GssApiBuffer> buffer, GssxxError error)
{
  std::cerr << "GssServerContext::receivedToken" << std::endl;

  if (error) {
    state_ = State::Error;
    error.setMessage("Error receiving token\n" + error.message());
    postCallback(error);
    return;
  }

  OM_uint32 majorStatus, minorStatus;
  gss_OID doid;

  auto sendBuffer = std::make_shared<GssExternalBuffer>();

  majorStatus = gss_accept_sec_context(&minorStatus, &context_, credential_, *buffer,
                                       GSS_C_NO_CHANNEL_BINDINGS, peerName_, &doid,
                                       *sendBuffer, &returnFlags_, nullptr, nullptr);

  switch (majorStatus) {
  case GSS_S_COMPLETE:
    state_ = State::Established;
    break;
  case GSS_S_CONTINUE_NEEDED:
    state_ = State::ContinueNeeded;
    break;
  default:
    state_ = State::Error;
    break;
  }

  if (state_ == State::Error) {
    postCallback({"Error accepting context.", majorStatus, minorStatus});
    return;
  }

  if (sendBuffer->size() > 0) {
    sendBuffer->sendAsync(*socketPtr_,
                          std::bind(&GssServerContext::sentToken,
                                    this,
                                    sendBuffer,
                                    std::placeholders::_1));
  } else if (state_ == State::ContinueNeeded) {
    auto receivedBuffer = std::make_shared<GssVectorBuffer>();
    receivedBuffer->receiveAsync(*socketPtr_,
                                 std::bind(&GssServerContext::receivedToken,
                                           this,
                                           receivedBuffer,
                                           std::placeholders::_1));
  } else {
    postCallback({});
  }
}

void
GssServerContext::sentToken(std::shared_ptr<GssApiBuffer> buffer, GssxxError error)
{
  // We need buffer to be passed into this function so that it retains the reference
  // until the send is complete.
  std::cerr << "GssServerContext::sentToken" << std::endl;

  if (error) {
    state_ = State::Error;
    error.setMessage("Error sending token.\n" + error.message());
    postCallback(error);
    return;
  }

  if (state_ == State::ContinueNeeded) {
    auto receivedBuffer = std::make_shared<GssVectorBuffer>();
    receivedBuffer->receiveAsync(*socketPtr_,
                                 std::bind(&GssServerContext::receivedToken,
                                           this,
                                           receivedBuffer,
                                           std::placeholders::_1));
  } else {
    postCallback({});
  }
}

void
GssServerContext::postCallback(const GssxxError& status)
{
  if (socketPtr_ == nullptr) {
    throw std::logic_error("GssServerContext::postCallback with a null socket");
  }

  if (callback_ == nullptr) {
    throw std::logic_error("GssServerContext::postCallback with a null callback");
  }

  auto& ioService = socketPtr_->get_io_service();
  auto callback = callback_;

  socketPtr_ = nullptr;
  callback_ = nullptr;

  ioService.post(std::bind(callback, status));
}

GssAuthData
GssServerContext::getAuthData()
{
  GssExternalBuffer buffer;
  OM_uint32 majorStatus, minorStatus;

  majorStatus = gsskrb5_extract_authz_data_from_sec_context(&minorStatus, context_, 1, buffer);
  if (majorStatus != GSS_S_COMPLETE) {
    throw GssException("Error extracting authdata", majorStatus, minorStatus);
  }

  return std::move(buffer);
}
