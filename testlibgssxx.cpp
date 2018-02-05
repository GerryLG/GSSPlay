#include <iostream>

#include "gssexception.hpp"
#include "gssname.hpp"
#include "gssservercred.hpp"

int main()
{
  std::cout << "starting..." << std::endl;

  try {
    gssxx::GssServerCredential svrCred{"sample", "/home/gerry/sample.keytab"};
  } catch (gssxx::GssException ex) {
    std::cerr << "Exception Caught: " << ex.what() << std::endl;
    std::cerr << ex.message() << std::endl;
  }

  std::cout << "done..." << std::endl;
}
