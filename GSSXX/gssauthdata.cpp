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

  auto authDataContainer = parser.parseItem();
  std::cerr << "Tag: " << std::string(authDataContainer->tag) << std::endl;
  std::cerr << "Length: " << authDataContainer->data.size() << std::endl;
  std::cerr << "ItemSize: " << authDataContainer->size << std::endl;

  parser.reset(&authDataContainer->data);
  auto authData1 = parser.parseItem();
  std::cerr << "Tag: " << std::string(authData1->tag) << std::endl;
  std::cerr << "Length: " << authData1->data.size() << std::endl;
  std::cerr << "ItemSize: " << authData1->size << std::endl;

  parser.reset(&authData1->data);
  auto authData1_0 = parser.parseItem();
  std::cerr << "Tag: " << std::string(authData1_0->tag) << std::endl;
  std::cerr << "Length: " << authData1_0->data.size() << std::endl;
  std::cerr << "ItemSize: " << authData1_0->size << std::endl;
  std::cerr << authData1_0->data << std::endl;

  auto authData1_1 = parser.parseItem();
  std::cerr << "Tag: " << std::string(authData1_1->tag) << std::endl;
  std::cerr << "Length: " << authData1_1->data.size() << std::endl;
  std::cerr << "ItemSize: " << authData1_1->size << std::endl;
  std::cerr << authData1_1->data << std::endl;

  parser.reset(&authData1_0->data);
  auto authData1_0val = parser.parseItem();
  std::cerr << "Tag: " << std::string(authData1_0val->tag) << std::endl;
  std::cerr << "Length: " << authData1_0val->data.size() << std::endl;
  std::cerr << "ItemSize: " << authData1_0val->size << std::endl;
  std::cerr << authData1_0val->data << std::endl;

  parser.reset(&authData1_0->data);
  auto authDataType = parser.parseInteger();
  std::cout << "AuthDataType: " << authDataType << std::endl;

  parser.reset(&authData1_1->data);
  auto authData1_1val = parser.parseItem();
  std::cerr << "Tag: " << std::string(authData1_1val->tag) << std::endl;
  std::cerr << "Length: " << authData1_1val->data.size() << std::endl;
  std::cerr << "ItemSize: " << authData1_1val->size << std::endl;
  std::cerr << authData1_1val->data << std::endl;
}
