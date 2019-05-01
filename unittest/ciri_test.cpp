#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest.h>
#include <ciri.hpp>
#include <random>

// ----------------------------------------------------------------------------
// Random generator utilities
// ----------------------------------------------------------------------------

// Function: random_engine
inline std::default_random_engine& random_engine() {
  thread_local static std::default_random_engine gen{0};
  return gen;
}

// Function: random
// Randomly generate a floating value in the given range.
template <typename T>
std::enable_if_t<std::is_floating_point<T>::value, T> random(
  const T from = -1.0, 
  const T to = 1.0
) {
  return std::uniform_real_distribution<T>(from, to)(random_engine());
}

// Function: random
// Randomly generate an integer value.
template <typename T>
std::enable_if_t<std::is_integral<T>::value, T> random(
  const T from = std::numeric_limits<T>::lowest(), 
  const T to = std::numeric_limits<T>::max()
) {
  return std::uniform_int_distribution<T>(from, to)(random_engine());
} 

// Function: random
// Randomly generate a string.
template <typename T>
std::enable_if_t<std::is_same<T, std::string>::value, T> random(
  const std::string::value_type from = ' ',
  const std::string::value_type to = '~',
  const std::string::size_type len = 16
) {
  std::string str(len, ' ');
  for(auto& c : str) {
    c = random<std::string::value_type>(from, to);
  }
  return str;
}

// Function: random
// Randomly generate a string.
template <typename T>
std::enable_if_t<std::is_same<T, std::wstring>::value, T> random(
  const std::wstring::value_type from = ' ',
  const std::wstring::value_type to = '~',
  const std::wstring::size_type len = 16
) {
  std::wstring str(len, ' ');
  for(auto& c : str) {
    c = random<std::wstring::value_type>(from, to);
  }
  return str;
}

// Function: random
// Randomly generate a string.
template <typename T>
std::enable_if_t<std::is_same<T, std::u16string>::value, T> random(
  const std::u16string::value_type from = ' ',
  const std::u16string::value_type to = '~',
  const std::u16string::size_type len = 16
) {
  std::u16string str(len, ' ');
  for(auto& c : str) {
    c = random<std::u16string::value_type>(from, to);
  }
  return str;
}

// Function: random
// Randomly generate a string.
template <typename T>
std::enable_if_t<std::is_same<T, std::u32string>::value, T> random(
  const std::u32string::value_type from = ' ',
  const std::u32string::value_type to = '~',
  const std::u32string::size_type len = 32
) {
  std::u32string str(len, ' ');
  for(auto& c : str) {
    c = random<std::u32string::value_type>(from, to);
  }
  return str;
}

// ----------------------------------------------------------------------------

// Struct: PODs
struct PODs {

  uint8_t       _bool   = random<decltype(_bool)>(); 
  char          _char   = random<decltype(_char)>();  
  unsigned char _uchar  = random<decltype(_uchar)>(); 
  uint8_t       _uint8  = random<decltype(_uint8)>();
  int8_t        _int8   = random<decltype(_int8)>();  
  uint16_t      _uint16 = random<decltype(_uint16)>();
  int16_t       _int16  = random<decltype(_int16)>(); 
  uint32_t      _uint32 = random<decltype(_uint32)>();
  int32_t       _int32  = random<decltype(_int32)>(); 
  uint64_t      _uint64 = random<decltype(_uint64)>();
  int64_t       _int64  = random<decltype(_int64)>(); 
  float_t       _float  = random<decltype(_float)>(); 
  double_t      _double = random<decltype(_double)>();

  template <typename ArchiverT>
  std::streamsize archive( ArchiverT& ar ) {
    return ar(
      _bool,  
      _char,  
      _uchar, 
      _uint8,
      _int8,  
      _uint16,
      _int16, 
      _uint32,
      _int32, 
      _uint64,
      _int64, 
      _float, 
      _double
    );
  }

  bool operator == (const PODs& rhs) const {
    return _bool   == rhs._bool   && 
           _char   == rhs._char   && 
           _uchar  == rhs._uchar  &&  
           _uint8  == rhs._uint8  &&  
           _int8   == rhs._int8   && 
           _uint16 == rhs._uint16 &&  
           _int16  == rhs._int16  && 
           _uint32 == rhs._uint32 && 
           _int32  == rhs._int32  && 
           _uint64 == rhs._uint64 && 
           _int64  == rhs._int64  && 
           _float  == rhs._float  && 
           _double == rhs._double; 
  }

  bool operator != (const PODs& rhs) const {
    return !(*this == rhs);
  }
}; 

// Procedure: test_pod
// The templated procedure for testing POD. Caller must specify the output 
// and input archiver type.
void test_pod() {

  // Output stream.
  std::ostringstream os;
  ciri::Ciri<std::ostream> oar(os);

  const auto o_bool   = random<int8_t>();
  const auto o_char   = random<char>(); 
  const auto o_uchar  = random<unsigned char>();
  const auto o_uint8  = random<uint8_t>();      
  const auto o_int8   = random<int8_t>();      
  const auto o_uint16 = random<uint16_t>();     
  const auto o_int16  = random<int16_t>();       
  const auto o_uint32 = random<uint32_t>();     
  const auto o_int32  = random<int32_t>();      
  const auto o_uint64 = random<uint64_t>();     
  const auto o_int64  = random<int64_t>();      
  const auto o_float  = random<float>();        
  const auto o_double = random<double>();       
  
  auto o_sz = oar(
    o_bool,  
    o_char,  
    o_uchar, 
    o_uint8,
    o_int8,  
    o_uint16,
    o_int16, 
    o_uint32,
    o_int32, 
    o_uint64,
    o_int64, 
    o_float, 
    o_double
  );

  //REQUIRE(o_sz == os.out_avail());

  // InputStreamBuffer
  std::istringstream is(os.str()); 
  ciri::Iric<std::istream> iar(is);

  auto i_bool   = random<int8_t>();
  auto i_char   = random<char>(); 
  auto i_uchar  = random<unsigned char>();
  auto i_uint8  = random<uint8_t>();      
  auto i_int8   = random<int8_t>();      
  auto i_uint16 = random<uint16_t>();     
  auto i_int16  = random<int16_t>();       
  auto i_uint32 = random<uint32_t>();     
  auto i_int32  = random<int32_t>();      
  auto i_uint64 = random<uint64_t>();     
  auto i_int64  = random<int64_t>();      
  auto i_float  = random<float>();        
  auto i_double = random<double>();       

  auto i_sz = iar(
    i_bool,  
    i_char,  
    i_uchar, 
    i_uint8,
    i_int8,  
    i_uint16,
    i_int16, 
    i_uint32,
    i_int32, 
    i_uint64,
    i_int64, 
    i_float, 
    i_double
  );
  REQUIRE(is.rdbuf()->in_avail() == 0);

  REQUIRE(i_sz == o_sz);
  REQUIRE(o_bool == i_bool);
  REQUIRE(o_char == i_char);
  REQUIRE(o_uchar == i_uchar);
  REQUIRE(o_uint8 == i_uint8);
  REQUIRE(o_int8 == i_int8);
  REQUIRE(o_uint16 == i_uint16);
  REQUIRE(o_int16 == i_int16);
  REQUIRE(o_uint32 == i_uint32);
  REQUIRE(o_int32 == i_int32);
  REQUIRE(o_uint64 == i_uint64);
  REQUIRE(o_int64 == i_int64);
  REQUIRE(o_float == i_float);
  REQUIRE(o_double == i_double);
}

// Procedure: test_struct
// The templated procedure for testing POD. Caller must specify the output 
// and input archiver type.
void test_struct() {

  for(size_t i=0; i<1024; ++i) {
    
    // POD struct.
    PODs o_pods;
    PODs i_pods;

    // Outputstream
    std::ostringstream os;
    ciri::Ciri<std::ostream> oar(os);
    auto o_sz = oar(o_pods);
    //REQUIRE(o_sz == os.out_avail());

    // Inputstream
    std::istringstream is(os.str());
    ciri::Iric<std::istream> iar(is);
    auto i_sz = iar(i_pods);
    REQUIRE(is.rdbuf()->in_avail() == 0);
    
    REQUIRE(o_sz == i_sz);
    REQUIRE(o_pods == i_pods);
  }
}

// Procedure: test_string
// Template for testing basic strings. Caller must specify the output and input archiver type.
template <typename T>
void test_string() {

  for(size_t i=0; i<4096; i++) {

    // Outputstream
    std::ostringstream os;
    ciri::Ciri<std::ostream> oar(os);

    T o_char_str = random<T>();
    auto o_sz = oar(o_char_str);
    //REQUIRE(o_sz == os.out_avail());

    // Inputstream
    std::istringstream is(os.str());
    ciri::Iric<std::istream> iar(is);
    
    T i_char_str;
    auto i_sz = iar(i_char_str);
    REQUIRE(is.rdbuf()->in_avail() == 0);

    REQUIRE(o_sz == i_sz);
    REQUIRE(o_char_str == i_char_str);
  }
}

#define TEST_SEQ_CONT_BODY(container)                          \
                                                               \
for(size_t i=0; i<1024; i++) {                                 \
  const size_t num_data = random<size_t>(1, 1024);             \
  std::ostringstream os;                                       \
  ciri::Ciri<std::ostream> oar(os);                            \
                                                               \
  std::container <int32_t>     o_int32s  (num_data);           \
  std::container <int64_t>     o_int64s  (num_data);           \
  std::container <char>        o_chars   (num_data);           \
  std::container <float>       o_floats  (num_data);           \
  std::container <double>      o_doubles (num_data);           \
  std::container <std::string> o_strings (num_data);           \
  std::container <PODs>        o_podses  (num_data);           \
                                                               \
  for(auto& v : o_int32s)  v = random<int32_t>();              \
  for(auto& v : o_int64s)  v = random<int64_t>();              \
  for(auto& v : o_chars)   v = random<char>();                 \
  for(auto& v : o_floats)  v = random<float>();                \
  for(auto& v : o_doubles) v = random<double>();               \
  for(auto& v : o_strings) v = random<std::string>();          \
                                                               \
  auto o_sz = oar(o_int32s, o_int64s, o_chars, o_floats, o_doubles, o_strings, o_podses);\
                                                               \
  std::istringstream is(os.str());                             \
  ciri::Iric<std::istream> iar(is);                            \
                                                               \
  std::container <int32_t>     i_int32s;                       \
  std::container <int64_t>     i_int64s;                       \
  std::container <char>        i_chars;                        \
  std::container <float>       i_floats;                       \
  std::container <double>      i_doubles;                      \
  std::container <std::string> i_strings;                      \
  std::container <PODs>        i_podses;                       \
                                                               \
  auto i_sz = iar(i_int32s, i_int64s, i_chars, i_floats, i_doubles, i_strings, i_podses);\
                                   \
  REQUIRE(o_sz == i_sz);           \
  REQUIRE(o_int32s == i_int32s);   \
  REQUIRE(o_int64s == i_int64s);   \
  REQUIRE(o_chars == i_chars);     \
  REQUIRE(o_floats == i_floats);   \
  REQUIRE(o_doubles == i_doubles); \
  REQUIRE(o_strings == i_strings); \
  REQUIRE(o_podses == i_podses);   \
}                                                                           


#define TEST_MAP_CONT_BODY(container)                                            \
                                                                                 \
for (size_t i = 0; i < 1024; i++) {                                              \
  const size_t num_data = random<size_t>(1, 1024);                               \
  std::ostringstream os;                                                         \
  ciri::Ciri<std::ostream> oar(os);                                              \
                                                                                 \
  std::container<int32_t, int32_t> o_int32s;                                     \
  std::container<int64_t, int64_t> o_int64s;                                     \
  std::container<char, char> o_chars;                                            \
  std::container<float, float> o_floats;                                         \
  std::container<double, double> o_doubles;                                      \
  std::container<std::string, std::string> o_strings;                            \
                                                                                 \
  for (size_t j = 0; j < num_data; j++) {                                        \
    o_int32s.emplace(random<int32_t>(), random<int32_t>());                      \
    o_int64s.emplace(random<int64_t>(), random<int64_t>());                      \
    o_chars.emplace(random<char>(), random<char>());                             \
    o_floats.emplace(random<float_t>(), random<float_t>());                      \
    o_doubles.emplace(random<double_t>(), random<double_t>());                   \
    o_strings.emplace(random<std::string>(), random<std::string>());             \
  }                                                                              \
                                                                                 \
  auto o_sz = oar(o_int32s, o_int64s, o_chars, o_floats, o_doubles , o_strings); \
                                                                                 \
  std::istringstream is(os.str());                                               \
  ciri::Iric<std::istream> iar(is);                                              \
                                                                                 \
  std::container<int32_t, int32_t> i_int32s;                                     \
  std::container<int64_t, int64_t> i_int64s;                                     \
  std::container<char, char> i_chars;                                            \
  std::container<float, float> i_floats;                                         \
  std::container<double, double> i_doubles;                                      \
  std::container<std::string, std::string> i_strings;                            \
                                                                                 \
  auto i_sz = iar(i_int32s, i_int64s, i_chars, i_floats, i_doubles , i_strings); \
  REQUIRE(0 == is.rdbuf()->in_avail());                                          \
                                                                                 \
  REQUIRE(o_sz == i_sz);                                                         \
  REQUIRE(o_int32s == i_int32s);                                                 \
  REQUIRE(o_int64s == i_int64s);                                                 \
  REQUIRE(o_chars == i_chars);                                                   \
  REQUIRE(o_floats == i_floats);                                                 \
  REQUIRE(o_doubles == i_doubles);                                               \
  REQUIRE(o_strings == i_strings);                                               \
}


#define TEST_SET_CONT_BODY(container)                                           \
                                                                                \
for (size_t i = 0; i < 1024; i++) {                                             \
  const size_t num_data = random<size_t>(1, 1024);                              \
  std::ostringstream os;                                                        \
  ciri::Ciri<std::ostream> oar(os);                                             \
                                                                                \
  std::container<int32_t> o_int32s;                                             \
  std::container<int64_t> o_int64s;                                             \
  std::container<char> o_chars;                                                 \
  std::container<float> o_floats;                                               \
  std::container<double> o_doubles;                                             \
  std::container<std::string> o_strings;                                        \
                                                                                \
  for (size_t j = 0; j < num_data; j++) {                                       \
    o_int32s.emplace(random<int32_t>());                                        \
    o_int64s.emplace(random<int64_t>());                                        \
    o_chars.emplace(random<char>());                                            \
    o_floats.emplace(random<float_t>());                                        \
    o_doubles.emplace(random<double_t>());                                      \
    o_strings.emplace(random<std::string>());                                   \
  }                                                                             \
  auto o_sz = oar(o_int32s, o_int64s, o_chars, o_floats, o_doubles, o_strings); \
                                                                                \
  std::istringstream is(os.str());                                              \
  ciri::Iric<std::istream> iar(is);                                             \
                                                                                \
  std::container<int32_t> i_int32s;                                             \
  std::container<int64_t> i_int64s;                                             \
  std::container<char> i_chars;                                                 \
  std::container<float> i_floats;                                               \
  std::container<double> i_doubles;                                             \
  std::container<std::string> i_strings;                                        \
                                                                                \
  auto i_sz = iar(i_int32s, i_int64s, i_chars, i_floats, i_doubles, i_strings); \
  REQUIRE(0 == is.rdbuf()->in_avail());                                         \
                                                                                \
  REQUIRE(o_sz == i_sz);                                                        \
  REQUIRE(o_int32s == i_int32s);                                                \
  REQUIRE(o_int64s == i_int64s);                                                \
  REQUIRE(o_chars == i_chars);                                                  \
  REQUIRE(o_floats == i_floats);                                                \
  REQUIRE(o_doubles == i_doubles);                                              \
  REQUIRE(o_strings == i_strings);                                              \
}

// Procedure: test_array
// Template procedure for testing array container.
void test_array() {

  for(size_t i=0; i<1024; ++i) {

    // Output
    std::array<char, 1> ochar;
    std::array<int, 512> oint;
    std::array<double, 1024> odouble;
    std::array<std::string, 2048> ostring;

    for(auto &i : ochar) i = random<char>();
    for(auto &i : oint) i = random<int>();
    for(auto &i : odouble) i = random<double>();
    for(auto &i : ostring) i = random<std::string>();

    std::ostringstream os;
    ciri::Ciri<std::ostream> oar(os);
    auto osz = oar(ochar, oint, odouble, ostring);

    // Input
    std::array<char, 1> ichar;
    std::array<int, 512> iint;
    std::array<double, 1024> idouble;
    std::array<std::string, 2048> istring;
    
    std::istringstream is(os.str());
    ciri::Iric<std::istream> iar(is);

    auto isz = iar(ichar, iint, idouble, istring);
    REQUIRE(0 == is.rdbuf()->in_avail());

    REQUIRE(osz == isz);
    REQUIRE(ochar == ichar);
    REQUIRE(oint == iint);
    REQUIRE(odouble == idouble);
    REQUIRE(ostring == istring);
  } 
}

// Procedure: test_variant
void test_variant() {

  for (size_t i = 0; i < 1024; i++) {            
    
    // Single POD variant.
    std::variant<int> opod1 = random<int>();
    std::variant<int> ipod1 = random<int>();

    // Multiple POD variant
    std::variant<int, double> opod2 = random<double>();
    std::variant<int, double> ipod2 = random<int>();
  
    // Multiple POD variant
    std::variant<int, double, bool> opod3 = random<int>()%2;
    std::variant<int, double, bool> ipod3 = random<double>();
    
    // Mixing float and string
    std::variant<float, std::string> omix2 = random<std::string>();
    std::variant<float, std::string> imix2 = random<float>();

    // Recursive variant
    std::variant<int, decltype(omix2)> orec2 = omix2;
    std::variant<int, decltype(omix2)> irec2 = random<int>();

    // Output archiver
    std::ostringstream os;
    ciri::Ciri<std::ostream> oar(os);
    auto osz = oar(opod1, opod2, opod3, omix2, orec2);
    
    // Input archiver
    std::istringstream is(os.str());
    ciri::Iric<std::istream> iar(is);
    auto isz = iar(ipod1, ipod2, ipod3, imix2, irec2);
    REQUIRE(0 == is.rdbuf()->in_avail());

    REQUIRE(osz == isz);
    REQUIRE(opod1 == ipod1);
    REQUIRE(opod2 == ipod2);
    REQUIRE(opod3 == ipod3);
    REQUIRE(omix2 == imix2);
    REQUIRE(orec2 == irec2);
  }
}

// Procedure: test_time_point
void test_time_point() {

  for(auto i=0; i<1024; ++i) {

    auto o_tpt1 = std::chrono::system_clock::now();
    auto o_tpt2 = std::chrono::steady_clock::now();
    auto o_tpt3 = std::chrono::high_resolution_clock::now();

    auto o_dur1 = std::chrono::system_clock::now() - o_tpt1;
    auto o_dur2 = std::chrono::steady_clock::now() - o_tpt2;
    auto o_dur3 = std::chrono::high_resolution_clock::now() - o_tpt3;
    
    // Output archiver
    std::ostringstream os;
    ciri::Ciri<std::ostream> oar(os);
    auto osz = oar(o_tpt1, o_tpt2, o_tpt3, o_dur1, o_dur2, o_dur3);

    decltype(o_tpt1) i_tpt1;
    decltype(o_tpt2) i_tpt2;
    decltype(o_tpt3) i_tpt3;
    decltype(o_dur1) i_dur1;
    decltype(o_dur2) i_dur2;
    decltype(o_dur3) i_dur3;

    // Input archiver
    std::istringstream is(os.str());
    ciri::Iric<std::istream> iar(is);
    auto isz = iar(i_tpt1, i_tpt2, i_tpt3, i_dur1, i_dur2, i_dur3);

    REQUIRE(0 == is.rdbuf()->in_avail());
    REQUIRE(osz == isz);

    REQUIRE(o_tpt1 == i_tpt1);
    REQUIRE(o_tpt2 == i_tpt2);
    REQUIRE(o_tpt3 == i_tpt3);
    REQUIRE(o_dur1 == i_dur1);
    REQUIRE(o_dur2 == i_dur2);
    REQUIRE(o_dur3 == i_dur3);
  }
}

// Procedure: test_optional
void test_optional() {

  for(auto i=0; i<1024; ++i) {

    std::optional<bool> o_nbool, i_nbool{true};
    std::optional<bool> o_ybool{true}, i_ybool;

    std::optional<std::string> o_nstr, i_nstr{random<std::string>()};
    std::optional<std::string> o_ystr{random<std::string>()}, i_ystr;

    // Output archiver
    std::ostringstream os;
    ciri::Ciri<std::ostream> oar(os);
    auto osz = oar(o_nbool, o_ybool, o_nstr, o_ystr);

    // Input archiver
    std::istringstream is(os.str());
    ciri::Iric<std::istream> iar(is);
    auto isz = iar(i_nbool, i_ybool, i_nstr, i_ystr);
    REQUIRE(0 == is.rdbuf()->in_avail());
    REQUIRE(osz == isz);

    REQUIRE(o_nbool == i_nbool);
    REQUIRE(o_ybool == i_ybool);
    REQUIRE(o_nstr == i_nstr);
    REQUIRE(o_ystr == i_ystr);
  }
}

// Procedure: test_tuple
void test_tuple() {

  for(auto i=0; i<1024; ++i) {

    std::tuple<> o0, i0;
    std::tuple<char> o1 {'a'}, i1 {'b'};
    std::tuple<int, double> o2 {1, 2.4}, i2 {3, 0.9};
    std::tuple<std::string, std::vector<int>, float> o3{"123", {1, 2, 3}, 4.5f}, i3;
    std::tuple<int, std::tuple<int, int>, int> o4 {1, {2, 3}, 4}, i4;

    // Output archiver
    std::ostringstream os;
    ciri::Ciri<std::ostream> oar(os);
    auto osz = oar(o0, o1, o2, o3, o4);

    // Input archiver
    std::istringstream is(os.str());
    ciri::Iric<std::istream> iar(is);
    auto isz = iar(i0, i1, i2, i3, i4);

    REQUIRE(0 == is.rdbuf()->in_avail());
    REQUIRE(osz == isz);

    REQUIRE(o0 == i0);
    REQUIRE(o1 == i1);
    REQUIRE(o2 == i2);
    REQUIRE(o3 == i3);
    REQUIRE(o4 == i4);
  }
}

// ----------------------------------------------------------------------------

// POD
TEST_CASE("POD" * doctest::timeout(60)) {
  test_pod();
}

// POD-struct
TEST_CASE("POD-Struct" * doctest::timeout(60)) {
  test_struct();
}

// std::string
TEST_CASE("string" * doctest::timeout(60)) {
  test_string<std::string>();
}

// std::wstring
TEST_CASE("wstring" * doctest::timeout(60)) {
  test_string<std::wstring>();
}

// std::u16string
TEST_CASE("u16string" * doctest::timeout(60)) {
  test_string<std::u16string>();
}

// std::u32string
TEST_CASE("u32string" * doctest::timeout(60)) {
  test_string<std::u32string>();
}

// std::vector
TEST_CASE("vector" * doctest::timeout(60)) {
  TEST_SEQ_CONT_BODY(vector)
}

// std::deque
TEST_CASE("deque" * doctest::timeout(60)) {
  TEST_SEQ_CONT_BODY(deque)
}

// std::list
TEST_CASE("list" * doctest::timeout(60)) {
  TEST_SEQ_CONT_BODY(list)
}

// std::forward_list
TEST_CASE("forward_list" * doctest::timeout(60)) {
  TEST_SEQ_CONT_BODY(forward_list)
}

// std::map
TEST_CASE("map" * doctest::timeout(60)) {
  TEST_MAP_CONT_BODY(map);
}

// std::unordered_map
TEST_CASE("unordered_map" * doctest::timeout(60)) {
  TEST_MAP_CONT_BODY(unordered_map);
}

// std::set
TEST_CASE("set" * doctest::timeout(60)) {
  TEST_SET_CONT_BODY(set);
}

// std::unordered_set
TEST_CASE("unordered_set" * doctest::timeout(60)) {
  TEST_SET_CONT_BODY(unordered_set);
}

// std::array
TEST_CASE("array" * doctest::timeout(60)) {
  test_array();
}

// std::chrono::time_point
TEST_CASE("time_point" * doctest::timeout(60)) {
  test_time_point();
}

// std::optional
TEST_CASE("optional" * doctest::timeout(60)) {
  test_optional();
}

// std::tuple
TEST_CASE("tuple" * doctest::timeout(60)) {
  test_tuple();
}
