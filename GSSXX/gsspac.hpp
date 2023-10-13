#ifndef GSSPAC_H
#define GSSPAC_H

#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>
#include <cctype>

#include <boost/endian/arithmetic.hpp>
#include <string>

#include "gssbuffer.hpp"
#include "gsslocalbuffer.hpp"
#include "gssresultbuffer.hpp"

namespace gssxx {

  class GssPac
  {
  public:
    //! Default constructor
    GssPac()
      : valid_ {false}
      , bufferPtr_ {nullptr}
    {
      std::cerr << "GssPac(DefaultConstructor)" << std::endl;
    }

    GssPac(const GssBuffer& buffer);

    ~GssPac()
    {
      std::cerr << "~GssPac()" << std::endl;
    }

    void save(const std::filesystem::path& filePath) const;

    const bool& Valid() const { return valid_; }
    
    const std::string& FullName() const { return fullName_; }
    const std::string& AccountName() const { return accountName_; }
    const std::string& LogonDomain() const { return logonDomain_; }
    const std::string& LogonServer() const { return logonServer_; }
    const std::string& DomainSid() const { return domainSid_; }
    const std::string& UserSid() const { return userSid_; }
    const std::string& DnsDomainName() const { return dnsDomainName_; }
    const std::string& UpnName() const { return upnName_; }

    const std::vector<std::string>& groupSids() const { return groupSids_; }

    const std::chrono::system_clock::time_point& LogonTime() const { return logonTime_; }
    const std::chrono::system_clock::time_point& LastPasswordChange() const { return lastPasswordChange_; }
    
  protected:
  private:
    std::shared_ptr<const GssBuffer> bufferPtr_;

    bool valid_;
    
    std::string fullName_;
    std::string accountName_;
    std::string logonDomain_;
    std::string logonServer_;
    std::string domainSid_;
    std::string userSid_;
    std::string dnsDomainName_;
    std::string upnName_;

    std::chrono::system_clock::time_point logonTime_;
    std::chrono::system_clock::time_point lastPasswordChange_;

    std::vector<std::string> groupSids_;
  };
}

#endif /* GSSPAC_H */
