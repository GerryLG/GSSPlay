#include <string>
#include <stdexcept>

#include "gssauthdata.hpp"
#include "gsspartialbuffer.hpp"
#include "gssvectorbuffer.hpp"
#include "gssder.hpp"

using namespace gssxx;

namespace {
  using namespace der;

  uint32_t
  ParseAuthDataTypeValue(const GssPartialBuffer& buffer)
  {
    DerParser parser {&buffer};
    auto authDataTypeValue = parser.parseInteger();
    return static_cast<uint32_t>(authDataTypeValue);
  }

  uint32_t
  ParseAuthDataType(const GssPartialBuffer& buffer)
  {
    DerParser parser {&buffer};
    auto authDataType = parser.parseItem();

    if (authDataType->tag != Tag {TagClass::Universal, TagPC::Primitive, 2}) {
        throw std::runtime_error {"ParseAuthDataType: expected Universal:Primitive:2"};
    }

    auto authDataTypeValue = ParseAuthDataTypeValue(authDataType->data);
    return authDataTypeValue;
  }

  std::unique_ptr<GssVectorBuffer>
  ParseAuthDataContentsValue(const GssPartialBuffer& buffer)
  {
    DerParser parser {&buffer};
    auto authDataContentsValue = parser.parseOctetString();
    return std::make_unique<GssVectorBuffer>(*authDataContentsValue);
  }
  
  std::unique_ptr<GssVectorBuffer>
  ParseAuthDataContents(const GssPartialBuffer& buffer)
  {
    DerParser parser {&buffer};
    auto authDataContents = parser.parseItem();

    if (authDataContents->tag != Tag {TagClass::Universal, TagPC::Primitive, 4}) {
        throw std::runtime_error {"ParseAuthDataContents: expected Universal:Primitive:4"};
    }

    auto authDataContentsValue = ParseAuthDataContentsValue(authDataContents->data);
    return authDataContentsValue;
  }

  GssAuthData
  ParseAuthDataItem(const GssPartialBuffer& buffer)
  {
    DerParser parser {&buffer};
    auto itemVec = parser.parseSequence();

    if (itemVec[0].tag != Tag {TagClass::ContextSpecific, TagPC::Constructed, 0}) {
        throw std::runtime_error {"ParseAuthDataItem: expected item 0 to be Contextspecific:Constructed:0"};
    }
    auto authDataType = ParseAuthDataType(itemVec[0].data);

    if (itemVec[1].tag != Tag {TagClass::ContextSpecific, TagPC::Constructed, 1}) {
        throw std::runtime_error {"ParseAuthDataItem: expected item 1 to be Contextspecific:Constructed:1"};
    }
    auto authDataContents = ParseAuthDataContents(itemVec[1].data);

    GssAuthData authData {authDataType, *authDataContents};
    return authData;
  }

  std::vector<GssAuthData>
  ParseAuthDataList(const GssPartialBuffer& buffer)
  {
    std::vector<GssAuthData> authDataVec;

    DerParser parser {&buffer};
    auto itemVec = parser.parseSequence();

    for (const auto& item : itemVec) {
      // Each item in the sequence should itself be a sequence
      std::cerr << "Parsing item" << std::endl;
      if (item.tag != Tag {TagClass::Universal, TagPC::Constructed, 16}) {
        throw std::runtime_error {"GssAuthdata::Parse(): expected a sequence."};
      }

      auto authDataItem = ParseAuthDataItem(item.data);
      authDataVec.push_back(std::move(authDataItem));
    }

    return authDataVec;
  }
}

std::vector<GssAuthData>
GssAuthData::Parse(const gssxx::GssBuffer& buffer)
{
  std::cerr << "GssAuthData::Parse(buffer)" << std::endl;
  std::cerr << buffer;

  using namespace der;

  DerParser parser {&buffer};
  auto authDataSeq = parser.parseItem();
  if (authDataSeq->tag != Tag {TagClass::Universal, TagPC::Constructed, 16}) {
    throw std::runtime_error {"GssAuthdata::Parse(): expected a sequence."};
  }

  auto results = ParseAuthDataList(authDataSeq->data);
  return results;
}
