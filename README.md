# Cpp-Ciri 

[![Standard](image/cpp17.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)

A fast modern C++ *header-only* serialization library

# Simple, Compact, and Extensible Serialization

```cpp
#include "ciri.hpp"                       // ciri is header-only

int main() {
  
  int from{100}, to{0};                   // data to serialize and deserialize

  std::ostringstream os;                  // create an output device with a write method
  ciri::Ciri ciri(os);                    // create a serializer "ciri"

  auto obytes = os(from);                 // serialize an integer
  
  std::istringstream is(os.str());        // create an input device with a read method
  ciri::Iric iric(is);                    // create a deserializer "iric"

  auto ibytes = is(to);                   // deserialize an integer
   
  assert(from == to && obytes == ibytes);
}
```
