#ifndef TALLOCROOTCONTEXT_H
#define TALLOCROOTCONTEXT_H

#include "gsstrace.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <talloc.h>

namespace gssxx {
  
  class TallocRootContext
  {
  public:
    //! Default constructor
    TallocRootContext()
    {
      ctx_ = talloc_new(nullptr);
      if (ctx_ == nullptr) {
        throw std::runtime_error("TallocRootContext(): talloc_new failed");
      }
      trace((std::ostringstream() << "TallocRootContext() => " << ctx_).str());
    }

    //! Copy constructor
    TallocRootContext(const TallocRootContext &other) = delete;

    //! Move constructor
    TallocRootContext(TallocRootContext &&other) noexcept
    {
      trace("TallocRootContext(move constructor)");
      ctx_ = other.ctx_;
      other.ctx_ = nullptr;
    }

    //! Destructor
    virtual ~TallocRootContext() noexcept
    {
      trace("~TallocRootContext()");
      doFree();
    }

    //! Copy assignment operator
    TallocRootContext& operator=(const TallocRootContext &other) = delete;

    //! Move assignment operator
    TallocRootContext& operator=(TallocRootContext &&other) noexcept
    {
      trace("TallocRootContext(move assignment)");
      doFree();
      ctx_ = other.ctx_;
      other.ctx_ = nullptr;

      return *this;
    }

    operator void*() {
      return ctx_;
    }

  protected:
  private:

    void doFree()
    {
      if (ctx_ != nullptr) {
        trace((std::ostringstream() << "TallocRootContext Freeing Context: " << ctx_).str());
        talloc_free(ctx_);
      }
    }

    void* ctx_;
  };
}

#endif /* TALLOCROOTCONTEXT_H */
