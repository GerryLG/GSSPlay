#include "gssclientcontext.hpp"
#include "gssexception.hpp"
#include "gssvectorbuffer.hpp"
#include "gssexternalbuffer.hpp"

using namespace gssxx;
using namespace boost;

using tcp = asio::ip::tcp;

void
gssxx::GssClientContext::InitiateContext(tcp::socket& socket, const std::string& peerName)
{
  std::cerr << "GssClientContext::InitiateContext()" << std::endl;

  peerName_ = peerName;

  OM_uint32 majorStatus, minorStatus;
  GssVectorBuffer receivedBuffer;
  GssExternalBuffer sendBuffer;

  do {
    majorStatus = gss_init_sec_context(&minorStatus,
                                       GSS_C_NO_CREDENTIAL,
                                       &context_,
                                       peerName_,
                                       GSS_C_NO_OID,
                                       GSS_C_MUTUAL_FLAG | GSS_C_REPLAY_FLAG,
                                       0,
                                       nullptr,
                                       receivedBuffer,
                                       nullptr,
                                       sendBuffer,
                                       &returnFlags_,
                                       nullptr);

    if (majorStatus != GSS_S_COMPLETE && majorStatus != GSS_S_CONTINUE_NEEDED) {
      throw GssException {"Error establishing context", majorStatus, minorStatus};
    }

    if (sendBuffer.size() > 0) {
      std::cerr << "Sending token:" << std::endl;
      std::cerr << sendBuffer << std::endl;
      sendBuffer.send(socket);
    }

    if (majorStatus == GSS_S_CONTINUE_NEEDED) {
      receivedBuffer.receive(socket);
      std::cerr << "Received Token:" << std::endl;
      std::cerr << receivedBuffer << std::endl;
    }
  } while (majorStatus == GSS_S_CONTINUE_NEEDED);
}
