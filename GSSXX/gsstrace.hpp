#ifndef GSSTRACE_H
#define GSSTRACE_H

#include <string_view>

#include "gssbuffer.hpp"

namespace gssxx {
  void setTrace(bool trace);
  bool getTrace();

  template <typename T>
  void trace(const T&  message)
  {
    if (getTrace()) {
      std::cerr << message << std::endl;
    }
  }
}

#endif /* GSSTRACE_H */

