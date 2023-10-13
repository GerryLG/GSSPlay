#ifndef TALLOCROOTCONTEXT_H
#define TALLOCROOTCONTEXT_H

#include <iostream>
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
      std::cerr << "TallocRootContext() =>" << ctx_ << std::endl;
    }

    //! Copy constructor
    TallocRootContext(const TallocRootContext &other) = delete;

    //! Move constructor
    TallocRootContext(TallocRootContext &&other) noexcept
    {
      std::cerr << "TallocRootContext(move constructor)" << std::endl;
      ctx_ = other.ctx_;
      other.ctx_ = nullptr;
    }

    //! Destructor
    virtual ~TallocRootContext() noexcept
    {
      std::cout << "~TallocRootContext()" << std::endl;
      doFree();
    }

    //! Copy assignment operator
    TallocRootContext& operator=(const TallocRootContext &other) = delete;

    //! Move assignment operator
    TallocRootContext& operator=(TallocRootContext &&other) noexcept
    {
      std::cerr << "TallocRootContext(move assignment)" << std::endl;
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
        std::cerr << "TallocRootContext Freeing Context: " << ctx_ << std::endl;
        talloc_free(ctx_);
      }
    }

    void* ctx_;
  };
}

#endif /* TALLOCROOTCONTEXT_H */
