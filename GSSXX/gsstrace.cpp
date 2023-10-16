#include "gsstrace.hpp"
#include "gssbuffer.hpp"
#include <string_view>

static bool isTrace { false };

namespace gssxx {
  void setTrace(bool trace)
  {
    isTrace = trace;
  }

  bool getTrace()
  {
    return isTrace;
  }
}
