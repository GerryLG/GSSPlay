#include "gssder.hpp"

using namespace gssxx::der;

Tag::operator std::string() const
{
  std::string classString;
  switch (tagClass) {
  case Class::Application:
    classString = "Application";
    break;
  case Class::ContextSpecific:
    classString = "ContextSpecific";
    break;
  case Class::Private:
    classString = "Private";
    break;
  case Class::Universal:
    classString = "Universal";
    break;
  }

  std::string pcString;
  switch (tagPc) {
  case PC::Primitive:
    pcString = "Primitive";
    break;
  case PC::Constructed:
    pcString = "Constructed";
    break;
  }

  return classString + ":" + pcString + ":" + std::to_string(tagNumber);
}

std::pair<std::size_t,Tag>
DerParser::parseTag(std::size_t offset)
{
  std::size_t tagSize {1};
  Tag tag;
  
  unsigned int c1 {bufferPtr_->charAt(offset)};

  switch (c1 & 0xC0) {
  case 0x00:
    tag.tagClass = der::Class::Universal;
    break;
  case 0x40:
    tag.tagClass = der::Class::Application;
    break;
  case 0x80:
    tag.tagClass = der::Class::ContextSpecific;
    break;
  case 0xC0:
    tag.tagClass = der::Class::Private;
    break;
  }
  
  switch (c1 & 0x20) {
  case 0x00:
    tag.tagPc = der::PC::Primitive;
    break;
  case 0x20:
    tag.tagPc = der::PC::Constructed;
    break;
  }

  if ((c1 & 0x1F) != 0x1f) {
    tag.tagNumber = (c1 & 0x1F);
  } else {
    tag.tagNumber = 0;
    do
      {
        tag.tagNumber <<= 7;
        c1 = bufferPtr_->charAt(offset + tagSize);
        ++tagSize;
        tag.tagNumber |= (c1 & 0x7f);
      } while ((c1 & 0x80) == 0x80);
  }

  return std::make_pair(tagSize,tag);
}

std::pair<std::size_t,std::size_t>
DerParser::parseLength(std::size_t offset)
{
  std::size_t lengthSize {1};
  std::size_t length {1};

  unsigned int c1 {bufferPtr_->charAt(offset)};

  if ((c1 & 0x80) == 0) {
    length = c1 & 0x7f;
  } else {
    lengthSize += c1 & 0x7f;
    length = 0;
    for (int i = 1; i < lengthSize; ++i) {
      length <<= 8;
      length += bufferPtr_->charAt(offset + i);
    }
  }

  return std::make_pair(lengthSize, length);
}

std::unique_ptr<DerItem>
DerParser::parseItem(std::size_t offset)
{
  std::size_t newOffset {offset};
  Tag tag;
  std::size_t objectSize;

  std::tie(objectSize, tag) = parseTag(offset);
  newOffset += objectSize;

  std::size_t length;
  std::tie(objectSize, length) = parseLength(offset);
  newOffset += objectSize;

  return std::make_unique<DerItem>(tag, GssPartialBuffer {*bufferPtr_, newOffset, length}, length);
  //  return std::unique_ptr<DerItem>{new DerItem {tag,
  //        {*bufferPtr_, newOffset, length}, length}};
}
