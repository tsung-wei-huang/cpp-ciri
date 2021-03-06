// A simple program that demonstrates how to use cpp-ciri

// 2019/04/30 - created by Tsung-Wei Huang

#include <sstream>
#include <cassert>
#include <iostream>

#include "ciri.hpp"  // cpp-ciri is header-only

// A custom struct
struct MyStruct {
  double x, y;
  
  // Define a save method to serialize data
  template <typename Archiver>
  auto save(Archiver& ar) const {
    return ar(x, y);
  } 
  
  // Define a load method to deserialize data
  template <typename Archiver>
  auto load(Archiver& ar) {
    return ar(x, y);
  }
};

int main() {
  
  // Data to serialize
  int oint {1};
  std::string ostr {"test"};
  MyStruct ostruct {1.0, 2.0};
  
  // serialization
  std::ostringstream oss;
  ciri::Serializer ciri(oss);
  auto num_output_bytes = ciri(oint, ostr, ostruct);
  std::cout << "serialized " << num_output_bytes << " bytes\n";  

  // Data to deserialize
  int iint;
  std::string istr;
  MyStruct istruct;

  // deserialization
  std::istringstream iss(oss.str());
  ciri::Deserializer iric(iss);
  auto num_input_bytes = iric(iint, istr, istruct);
  std::cout << "deserialized " << num_input_bytes << " bytes\n";
  
  // assertion check
  assert(num_input_bytes == num_output_bytes);
  assert(oint == iint);
  assert(ostr == istr);
  assert(ostruct.x == istruct.x);
  assert(ostruct.y == istruct.y);

  return 0;
}
