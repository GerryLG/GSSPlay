#include "gssder.hpp"

using namespace gssxx::der;

Tag::operator std::string() const
{
  std::string classString;
  switch (tagClass) {
  case TagClass::Application:
    classString = "Application";
    break;
  case TagClass::ContextSpecific:
    classString = "ContextSpecific";
    break;
  case TagClass::Private:
    classString = "Private";
    break;
  case TagClass::Universal:
    classString = "Universal";
    break;
  }

  std::string pcString;
  switch (tagPc) {
  case TagPC::Primitive:
    pcString = "Primitive";
    break;
  case TagPC::Constructed:
    pcString = "Constructed";
    break;
  }

  return classString + ":" + pcString + ":" + std::to_string(tagNumber);
}

std::pair<std::size_t,Tag>
DerParser::parseTag()
{
  std::size_t tagSize {1};
  Tag tag;
  
  unsigned int c1 {bufferPtr_->charAt(offset_)};

  switch (c1 & 0xC0) {
  case 0x00:
    tag.tagClass = der::TagClass::Universal;
    break;
  case 0x40:
    tag.tagClass = der::TagClass::Application;
    break;
  case 0x80:
    tag.tagClass = der::TagClass::ContextSpecific;
    break;
  case 0xC0:
    tag.tagClass = der::TagClass::Private;
    break;
  }
  
  switch (c1 & 0x20) {
  case 0x00:
    tag.tagPc = der::TagPC::Primitive;
    break;
  case 0x20:
    tag.tagPc = der::TagPC::Constructed;
    break;
  }

  if ((c1 & 0x1F) != 0x1f) {
    tag.tagNumber = (c1 & 0x1F);
  } else {
    tag.tagNumber = 0;
    do
      {
        tag.tagNumber <<= 7;
        c1 = bufferPtr_->charAt(offset_ + tagSize);
        ++tagSize;
        tag.tagNumber |= (c1 & 0x7f);
      } while ((c1 & 0x80) == 0x80);
  }

  std::cerr << "Parsed Tag:" << std::endl;
  std::cerr << GssPartialBuffer(*bufferPtr_, offset_, tagSize);
  std::cerr << std::string(tag) << std::endl;
  
  offset_ += tagSize;
  return std::make_pair(tagSize,tag);
}

std::pair<std::size_t,std::size_t>
DerParser::parseLength()
{
  std::size_t lengthSize {1};
  std::size_t length {1};

  unsigned int c1 {bufferPtr_->charAt(offset_)};

  if ((c1 & 0x80) == 0) {
    length = c1 & 0x7f;
  } else {
    lengthSize += c1 & 0x7f;
    length = 0;
    for (int i = 1; i < lengthSize; ++i) {
      length <<= 8;
      length += bufferPtr_->charAt(offset_ + i);
    }
  }

  std::cerr << "Parsed Length:" << std::endl;
  std::cerr << GssPartialBuffer(*bufferPtr_, offset_, lengthSize);
  std::cerr << "Length: " << length << std::endl;

  offset_ += lengthSize;
  return std::make_pair(lengthSize, length);
}

std::unique_ptr<DerItem>
DerParser::parseItem()
{
  std::cerr << "DerParser::parseItem()" << std::endl;

  std::size_t newOffset {offset_};
  Tag tag;
  std::size_t objectSize;

  std::tie(objectSize, tag) = parseTag();
  newOffset += objectSize;

  std::size_t length;
  std::tie(objectSize, length) = parseLength();
  newOffset += objectSize;

  auto itemSize = (newOffset - offset_) + length;

  offset_ = newOffset + length;
  return std::make_unique<DerItem>(tag, GssPartialBuffer {*bufferPtr_, newOffset, length}, itemSize);
}

std::vector<DerItem>
DerParser::parseSequence()
{
  std::cerr << "DerParser::parseSequence()" << std::endl;

  std::vector<DerItem> retItems;

  while (remaining()) {
    retItems.push_back(*parseItem());
  }

  return retItems;
}

long
DerParser::parseInteger()
{
  std::cerr << "DerParser::parseInteger()" << std::endl;

  long retVal {0};
  if ((bufferPtr_->charAt(0) & 0x80) == 0x80) {
    retVal = -1;
  }

  for (std::size_t i {0}; i < bufferPtr_->size(); ++i) {
    retVal <<= 8;
    retVal |= bufferPtr_->charAt(i);
  }

  std::cerr << "Parsed Integer:" << std::endl;
  std::cerr << *bufferPtr_;
  std::cerr << "Integer: " << retVal << std::endl;

  return retVal;
}

std::unique_ptr<gssxx::GssPartialBuffer>
DerParser::parseOctetString()
{
  std::cerr << "DerParser::parseOctetString()" << std::endl;

  return std::make_unique<gssxx::GssPartialBuffer>(*bufferPtr_, 0, bufferPtr_->size());
}
