#ifndef GSSXXERROR_H
#define GSSXXERROR_H

#include <string>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <gssapi/gssapi.h>

#include "gssexception.hpp"
#include "gssstatus.hpp"

namespace gssxx {

  // TODO: Add some way of adding to message.
  class GssxxError {
  public:
    enum class ErrorType {
      NoError,
      GssError,
      SystemError
    };

    GssxxError()
      : type_ {ErrorType::NoError}
      , message_ {"No Error"}
      , gssStatus_ {}
      , errorCode_ {}
    {
    }

    GssxxError(std::string message, OM_uint32 majorStatus, OM_uint32 minorStatus)
      : type_ {ErrorType::GssError}
      , message_ {message}
      , gssStatus_ {majorStatus, minorStatus}
      , errorCode_ {}
    {
    }

    GssxxError(std::string message, const boost::system::error_code& errorCode)
      : type_ {ErrorType::SystemError}
      , message_ {message}
      , gssStatus_ {}
      , errorCode_ {errorCode}
    {
    }

    operator bool() const
    {
      return type_ != ErrorType::NoError;
    }

    ErrorType errorType() const
    {
      return type_;
    }

    const GssStatus& gssStatus() const
    {
      return gssStatus_;
    }

    const boost::system::error_code& systemError() const
    {
      return errorCode_;
    }

    std::string message() const
    {
      return message_;
    }

    void setMessage(const std::string& message)
    {
      message_ = message;
    }

    void throwExceptionIfError()
    {
      switch (type_) {
      case ErrorType::GssError:
        throw GssException {message_, gssStatus_};
        break;
      case ErrorType::SystemError:
        throw boost::system::system_error {errorCode_, message_};
        break;
      default:
        break;
      }
    }
    
  private:
    ErrorType type_;
    std::string message_;
    GssStatus gssStatus_;
    boost::system::error_code errorCode_;
  };

}  // gssxx

#endif /* GSSXXERROR_H */
