#include <algorithm>

#include "gssstatus.hpp"

using namespace gssxx;

static std::vector<std::string>
getMessage(OM_uint32 code, int type)
{
  OM_uint32 majorStatus, minorStatus;
  OM_uint32 messageContext;
  gss_buffer_desc message;

  std::vector<std::string> messages;

  messageContext = 0;
  while (true) {
    majorStatus = gss_display_status(&minorStatus, code, type, GSS_C_NULL_OID,
                                     &messageContext, &message);
    if (majorStatus == GSS_S_COMPLETE) {
      messages.push_back(std::string{(char *)message.value});
    } else {
      messages.push_back("*** Error calling gss_display_status ***");
      auto messages2 = getMessage(majorStatus, GSS_C_GSS_CODE);
      std::copy(messages2.begin(), messages2.end(),
                std::back_inserter(messages));
      messages2 = getMessage(minorStatus, GSS_C_MECH_CODE);
      std::copy(messages2.begin(), messages2.end(),
                std::back_inserter(messages));
    }
    gss_release_buffer(&minorStatus, &message);
    if (!messageContext) {
      break;
    }
  }

  return messages;
}

std::vector<std::string>
GssStatus::majorMessage() const
{
  return getMessage(majorStatus_, GSS_C_GSS_CODE);
}

std::vector<std::string>
GssStatus::minorMessage() const
{
  return getMessage(minorStatus_, GSS_C_MECH_CODE);
}

std::string
GssStatus::message() const
{
  std::string returnMessage;
  for (auto m : majorMessage()) {
    if (returnMessage.length()) {
      returnMessage += "\n";
    }
    returnMessage += m;
  }
  for (auto m : minorMessage()) {
    if (returnMessage.length()) {
      returnMessage += "\n";
    }
    returnMessage += m;
  }
  return returnMessage;
}
