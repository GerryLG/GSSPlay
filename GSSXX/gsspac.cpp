#include <chrono>
#include <filesystem>
#include <iostream>
#include <memory>
#include <ratio>
#include <sstream>
#include <stdexcept>
#include <string>

#include <boost/endian/arithmetic.hpp>

#include <talloc.h>

#include "gsslocalbuffer.hpp"
#include "gsspac.hpp"
#include "tallocrootcontext.hpp"

extern "C" {
#include <ndr.h>
#include <ndr/ndr_krb5pac.h>
#include <gen_ndr/krb5pac.h>
#include <util/data_blob.h>
#include <util/time.h>
}

static std::chrono::system_clock::time_point
to_time_point(NTTIME nttime)
{
  using namespace std::chrono;
  using namespace std::literals;

  // nttime is in 100 nanosecond units from an epoch of 1/Jan/1601
  duration<NTTIME, std::micro> nttime_usecs {nttime / 10};
  duration<NTTIME, std::ratio<86400, 1>> nttime_offset_days { 134774 };
  auto time_point =
      std::chrono::system_clock::time_point{nttime_usecs - nttime_offset_days};
  return time_point;

  // auto epoch = sys_days{1601y / January / 1};
  // auto epochdiff = epoch.time_since_epoch();
  // return epoch + nttime_usecs;
}

static std::string
to_sid_string(struct dom_sid* sid, long long rid = -1)
{
  std::ostringstream sidString;
  auto idAuth = *reinterpret_cast<boost::endian::big_uint48_t*>(sid->id_auth);
  sidString << "S-" << static_cast<unsigned>(sid->sid_rev_num) << "-" << idAuth;

  auto subAuthCount = sid->num_auths;
  for (auto i = 0; i < subAuthCount; ++i) {
    sidString << "-" << sid->sub_auths[i];
  }

  if (rid >= 0) {
    sidString << "-" << rid;
  }

  return sidString.str();
}

namespace gssxx {
  GssPac::GssPac(const GssBuffer& buffer)
    : bufferPtr_ { std::make_shared<GssLocalBuffer>(buffer) }
  {
    std::cerr << "GssPac(const GssBuffer&)" << std::endl;

    TallocRootContext ctx;
    auto pac_data_blob = data_blob_talloc(ctx, bufferPtr_->data(), bufferPtr_->size());
    auto pac_data = talloc(ctx, struct PAC_DATA);
    auto ndr_err = ndr_pull_struct_blob(&pac_data_blob,
                                        pac_data,
                                        pac_data,
                                        reinterpret_cast<ndr_pull_flags_fn_t>(ndr_pull_PAC_DATA));

    if ( ! NDR_ERR_CODE_IS_SUCCESS(ndr_err) ) {
      throw std::runtime_error("Unable to parse PAC");
    }

    for (auto i=0; i < pac_data->num_buffers; ++i) {
      auto buffer = pac_data->buffers[i];

      switch (buffer.type) {
      case PAC_TYPE_LOGON_INFO:
        {
          std::cerr << "Processing Buffer Type: " << buffer.type << std::endl;
          auto logonInfo = buffer.info->logon_info.info->info3;
          fullName_    = logonInfo.base.full_name.string;
          accountName_ = logonInfo.base.account_name.string;
          logonDomain_ = logonInfo.base.logon_domain.string;
          logonServer_ = logonInfo.base.logon_server.string;
          logonTime_   = to_time_point(logonInfo.base.logon_time);
          lastPasswordChange_ = to_time_point(logonInfo.base.last_password_change);

          auto domainSidPtr = logonInfo.base.domain_sid;
          
          domainSid_ = to_sid_string(domainSidPtr);
          userSid_ = to_sid_string(domainSidPtr, logonInfo.base.rid);

          auto groupCount = logonInfo.base.groups.count;
          groupSids_.reserve(groupCount);
          for (auto i = 0; i < groupCount; ++i) {
            groupSids_.push_back(to_sid_string(domainSidPtr, logonInfo.base.groups.rids[i].rid));
          }
          
          break;
        }
      case PAC_TYPE_UPN_DNS_INFO:
        {
          std::cerr << "Processing Buffer Type: " << buffer.type << std::endl;
          auto dnsInfo = buffer.info->upn_dns_info;
          dnsDomainName_ = dnsInfo.dns_domain_name;
          upnName_ = dnsInfo.upn_name;
          break;
        }
      case PAC_TYPE_LOGON_NAME:
      case PAC_TYPE_SRV_CHECKSUM:
      case PAC_TYPE_KDC_CHECKSUM:
        // Ignore
        break;
      default:
        std::cerr << "Ignoring Buffer Type: " << buffer.type << std::endl;
        break;
      }
    }

    valid_ = true;
  }

  void GssPac::save(const std::filesystem::path& filePath) const
  {
    std::cerr << "GssPac::save()" << std::endl;

    if ( ! valid_ ) {
      throw std::runtime_error("Cannot save an invalid PAC");
    }

    bufferPtr_->save(filePath);
  }
}
