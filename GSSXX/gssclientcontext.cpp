#include "gssclientcontext.hpp"
#include "gssexception.hpp"
#include "gsslocalbuffer.hpp"
#include "gssresultbuffer.hpp"

using namespace gssxx;
using namespace boost;

using tcp = asio::ip::tcp;

void
gssxx::GssClientContext::InitiateContext(tcp::socket& socket, const std::string& peerName)
{
  trace("GssClientContext::InitiateContext()");

  peerName_ = peerName;

  OM_uint32 majorStatus, minorStatus;
  GssLocalBuffer receivedBuffer;
  GssResultBuffer sendBuffer;

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
      trace("Sending token:");
      trace(sendBuffer);
      sendBuffer.send(socket);
    }

    if (majorStatus == GSS_S_CONTINUE_NEEDED) {
      receivedBuffer.receive(socket);
      trace("Received Token:");
      trace(receivedBuffer);
    }
  } while (majorStatus == GSS_S_CONTINUE_NEEDED);
}
