#include <string>
#include <stdexcept>

#include "gssauthdata.hpp"
#include "gsspartialbuffer.hpp"
#include "gssder.hpp"

using namespace gssxx;

void
GssAuthData::dumpTest() const
{
  der::DerParser parser {&buffer_};;
  std::size_t offset {0};
  der::Tag tag;

  std::size_t objectSize;
  std::tie(objectSize, tag) = parser.parseTag(offset);
  
  std::cerr << GssPartialBuffer {buffer_, offset, objectSize};
  std::cerr << static_cast<std::string>(tag) << std::endl << std::endl;
  offset += objectSize;

  if (tag.tagNumber != 16 && tag.tagClass != der::TagClass::Universal && tag.tagPc != der::TagPC::Constructed) {
    throw std::runtime_error(std::string {"Invalid tag found, "} + static_cast<std::string>(tag));
  }

  std::size_t length;
  std::tie(objectSize, length) = parser.parseLength(offset);
  std::cerr << GssPartialBuffer {buffer_, offset, objectSize};
  std::cerr << "Length: " << length << std::endl << std::endl;
  offset += objectSize;

  std::cerr << "Remaining: " << buffer_.size() - (offset + length) << std::endl;
}

// std::vector<der::DerItem>
// GssAuthData::derParseSequenceOfSequence(const gssxx::GssBuffer &sosBuffer)
// {
//   std::vector<der::DerItem> sequences;

//   std::size_t offset {0};
//   der::Tag tag;

//   std::size_t objectSize;
//   objectSize = derParseTag(sosBuffer, offset, tag);
//   offset += objectSize;

//   if (tag != der::Tag {der::Class::Universal, der::PC::Constructed, 16}) {
//     throw std::runtime_error(std::string {"Invalid tag found, "} + static_cast<std::string>(tag));
//   }

//   std::size_t length;
//   objectSize = derParseLength(sosBuffer, offset, length);
//   offset += objectSize;

//   auto sequencesBuffer = GssPartialBuffer {sosBuffer, offset, length};

//   offset = 0;
//   while (offset < sequencesBuffer.size()) {
    
//   }

//   return sequences;
// }
