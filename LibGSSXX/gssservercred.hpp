#ifndef GSSSERVERCRED_H
#define GSSSERVERCRED_H

#include <string>
#include <utility>

#include <gssapi/gssapi.h>

namespace gssxx {

  class GssServerCredential;

  inline void swap(GssServerCredential& lhs, GssServerCredential& rhs) noexcept;

  class GssServerCredential {
    friend void swap(GssServerCredential& lhs, GssServerCredential& rhs) noexcept;

  public:
    GssServerCredential(const std::string& serviceName);
    GssServerCredential(const std::string& serviceName, const std::string& keytabFile);

    //! Copy constructor
    GssServerCredential(const GssServerCredential& other) noexcept
      : credential_ {other.credential_}
      , referenceCount_ {other.referenceCount_}
    {
      ++*referenceCount_;
    }

    //! Destructor
    virtual ~GssServerCredential() noexcept;

    //! Copy assignment operator
    GssServerCredential& operator=(GssServerCredential other)
    {
      swap(*this, other);
      return *this;
    }

    operator gss_cred_id_t&()
    {
      return credential_;
    }

  private:
    void setCredential(const std::string& serviceName);
    gss_cred_id_t credential_;
    int* referenceCount_;
  };

  inline void swap(GssServerCredential& lhs, GssServerCredential& rhs) noexcept
  {
    using std::swap;
    swap(lhs.credential_, rhs.credential_);
    swap(lhs.referenceCount_, rhs.referenceCount_);
  }
}  // gssxx

#endif /* GSSSERVERCRED_H */
