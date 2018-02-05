#ifndef GSSNAME_H
#define GSSNAME_H

#include <string>
#include <gssapi/gssapi.h>

namespace gssxx
{
  class GssName
  {
    friend void swap(GssName& first, GssName& second);
  public:
    GssName(const std::string& name);
    GssName(const gss_name_t& name);

    //! Copy constructor
    GssName(const GssName &other);

    //! Move constructor
    GssName(GssName &&other) noexcept
    {
      name_ = other.name_;
      other.name_ = nullptr;
    }

    //! Destructor
    virtual ~GssName() noexcept;

    //! assignment operator
    GssName& operator=(GssName other)
    {
      swap(*this, other);
      return *this;
    }

    std::string to_string() const;

    operator std::string() const {
      return to_string();
    }

    operator gss_name_t&()
    {
      return name_;
    }

    operator gss_name_t*()
    {
      return &name_;
    }

  private:
    gss_name_t name_;
  };

  inline void swap(GssName& first, GssName& second)
  {
    using std::swap;
    swap(first.name_, second.name_);
  }
}  // gssxx

#endif /* GSSNAME_H */
