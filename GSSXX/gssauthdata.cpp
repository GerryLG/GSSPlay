#include <string>
#include <stdexcept>

#include "gssauthdata.hpp"
#include "gsspartialbuffer.hpp"
#include "gssvectorbuffer.hpp"
#include "gssder.hpp"

using namespace gssxx;

std::vector<GssAuthData>
GssAuthData::Parse(const gssxx::GssBuffer& buffer)
{
  std::cerr << "GssAuthData::Parse(buffer)" << std::endl;
  std::cerr << buffer;

  using namespace der;

  DerParser parser {&buffer};
  auto itemVec = parser.parseSequence();
  std::cerr << "Buffer contains " << itemVec.size() << " items" << std::endl;

  std::vector<GssAuthData> authDataVec;
  for (const auto& item : itemVec) {
    std::cerr << "Parsing item" << std::endl;
    if (item.tag != Tag {TagClass::Universal, TagPC::Constructed, 16}) {
      throw std::runtime_error {"GssAuthdata::Parse(): expected a sequence."};
    }
    // Parse the sequence: should be authdatatype then data.
    DerParser authDataParser {&item.data};
    auto adTypeItem = authDataParser.parseItem();
    std::cerr << std::string(adTypeItem->tag) << std::endl;
    std::cerr << adTypeItem->data;

    DerParser adTypeParser {&adTypeItem->data};
    auto adType = adTypeParser.parseInteger();
    std::cerr << "AuthData Type: " << adType << std::endl;
    
    auto adDataItem = authDataParser.parseItem();
    std::cerr << std::string(adDataItem->tag) << std::endl;

    DerParser adDataParser {&adDataItem->data};
    auto adData = adDataParser.parseOctetString();
    authDataVec.push_back({static_cast<uint32_t>(adType), *adData});
  }

  return authDataVec;
}
