#ifndef GSSDER_H
#define GSSDER_H

#include <string>
#include <memory>

#include "gsspartialbuffer.hpp"

namespace gssxx {
  
  namespace der {
    enum class TagClass {
      Universal = 0,
      Application = 1,
      ContextSpecific = 2,
      Private = 3
    };

    enum class TagPC {
      Primitive = 0,
      Constructed = 1
    };

    struct Tag {
      TagClass tagClass;
      TagPC tagPc;
      int tagNumber;
      operator std::string() const;
      bool operator==(const Tag& other) const
      {
        return tagClass == other.tagClass && tagPc == other.tagPc && tagNumber == other.tagNumber;
      }
      bool operator!=(const Tag& other) const
      {
        return ! (*this == other);
      }
    };

    struct DerItem {
      DerItem(Tag newTag, GssPartialBuffer newBuffer, std::size_t newSize)
        : tag {newTag}
        , data(std::move(newBuffer))
        , size {newSize}
      {
      }

      DerItem(const DerItem& other) = default;
      DerItem(DerItem&& other) = default;
      DerItem& operator=(const DerItem& other) = default;
      DerItem& operator=(DerItem&& other) = default;
      
      Tag tag;
      GssPartialBuffer data;
      std::size_t size;
    };

    class DerParser {
    public:
      DerParser(const GssBuffer* bufferPtr)
        : bufferPtr_ {bufferPtr}
        , offset_ {0}
      {
      }

      void reset(const GssBuffer* bufferPtr)
      {
        bufferPtr_ = bufferPtr;
        offset_ = 0;
      }

      bool remaining()
      {
        return offset_ < bufferPtr_->size();
      }

      std::unique_ptr<DerItem> parseItem();
      std::vector<DerItem> parseSequence();
      long parseInteger();
      std::unique_ptr<GssPartialBuffer> parseOctetString();

    private:
      std::pair<std::size_t,der::Tag> parseTag();
      std::pair<std::size_t,std::size_t> parseLength();

      const GssBuffer* bufferPtr_;
      std::size_t offset_;
    };
  }
}  // gssxx
 
#endif /* GSSDER_H */
