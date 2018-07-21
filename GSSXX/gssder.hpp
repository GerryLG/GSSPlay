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
      bool operator==(const Tag& other)
      {
        return tagClass == other.tagClass && tagPc == other.tagPc && tagNumber == other.tagNumber;
      }
      bool operator!=(const Tag& other)
      {
        return ! (*this == other);
      }
    };

    struct DerItem {
      DerItem(Tag newTag, GssPartialBuffer newBuffer)
        : tag {newTag}
        , data(std::move(newBuffer))
      {
      }
      
      Tag tag;
      GssPartialBuffer data;
    };

    class DerParser {
    public:
      DerParser(const GssBuffer* bufferPtr)
        : bufferPtr_{bufferPtr}
      {
      }

      std::pair<std::size_t,der::Tag> parseTag(std::size_t offset);
      std::pair<std::size_t,std::size_t> parseLength(std::size_t offset);
      std::unique_ptr<DerItem> parseItem(std::size_t offset = 0);

    private:
      const GssBuffer* bufferPtr_;
    };
  }
}  // gssxx
 
#endif /* GSSDER_H */
