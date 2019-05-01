// Cpp-Ciri: A Fast Serialization Library in Modern C++
//
// MIT License: Copyright (c) 2019 Tsung-Wei Huang and Chun-Xun Lin
//

#pragma once

#include <tuple>
#include <utility>
#include <string>
#include <vector>
#include <list>
#include <forward_list>
#include <queue>
#include <stack>
#include <type_traits>
#include <memory>
#include <map>
#include <set>
#include <variant>
#include <chrono>
#include <any>
#include <unordered_map>
#include <unordered_set>
#include <system_error>

namespace ciri {

// ----------------------------------------------------------------------------
// Supported C++ STL type
// ----------------------------------------------------------------------------

template <typename T> struct is_std_basic_string : std::false_type {};
template <typename... ArgsT> struct is_std_basic_string <std::basic_string<ArgsT...>> : std::true_type {};
template <typename T> constexpr bool is_std_basic_string_v = is_std_basic_string<T>::value;

template <typename T> struct is_std_array : std::false_type {};
template <typename T, size_t N> struct is_std_array <std::array<T, N>> : std::true_type {};
template <typename T> constexpr bool is_std_array_v = is_std_array<T>::value;

template <typename T> struct is_std_vector : std::false_type {};
template <typename... ArgsT> struct is_std_vector <std::vector<ArgsT...>> : std::true_type {};
template <typename T> constexpr bool is_std_vector_v = is_std_vector<T>::value;

template <typename T> struct is_std_deque : std::false_type {};
template <typename... ArgsT> struct is_std_deque <std::deque<ArgsT...>> : std::true_type {};
template <typename T> constexpr bool is_std_deque_v = is_std_deque<T>::value;

template <typename T> struct is_std_list : std::false_type {};
template <typename... ArgsT> struct is_std_list <std::list<ArgsT...>> : std::true_type {};
template <typename T> constexpr bool is_std_list_v = is_std_list<T>::value;

template <typename T> struct is_std_forward_list : std::false_type {};
template <typename... ArgsT> struct is_std_forward_list <std::forward_list<ArgsT...>> : std::true_type {};
template <typename T> constexpr bool is_std_forward_list_v = is_std_forward_list<T>::value;

template <typename T> struct is_std_map : std::false_type {};
template <typename... ArgsT> struct is_std_map <std::map<ArgsT...>> : std::true_type {};
template <typename T> constexpr bool is_std_map_v = is_std_map<T>::value;

template <typename T> struct is_std_unordered_map : std::false_type {};
template <typename... ArgsT> struct is_std_unordered_map <std::unordered_map<ArgsT...>> : std::true_type {};
template <typename T> constexpr bool is_std_unordered_map_v = is_std_unordered_map<T>::value;

template <typename T> struct is_std_set : std::false_type {};
template <typename... ArgsT> struct is_std_set <std::set<ArgsT...>> : std::true_type {};
template <typename T> constexpr bool is_std_set_v = is_std_set<T>::value;

template <typename T> struct is_std_unordered_set : std::false_type {};
template <typename... ArgsT> struct is_std_unordered_set <std::unordered_set<ArgsT...>> : std::true_type {};
template <typename T> constexpr bool is_std_unordered_set_v = is_std_unordered_set<T>::value;

template <typename T> struct is_std_variant : std::false_type {};
template <typename... ArgsT> struct is_std_variant <std::variant<ArgsT...>> : std::true_type {};
template <typename T> constexpr bool is_std_variant_v = is_std_variant<T>::value;

template <typename T> struct is_std_optional : std::false_type {};
template <typename... ArgsT> struct is_std_optional <std::optional<ArgsT...>> : std::true_type {};
template <typename T> constexpr bool is_std_optional_v = is_std_optional<T>::value;

template <typename T> struct is_std_unique_ptr : std::false_type {};
template <typename... ArgsT> struct is_std_unique_ptr <std::unique_ptr<ArgsT...>> : std::true_type {};
template <typename T> constexpr bool is_std_unique_ptr_v = is_std_unique_ptr<T>::value;

template <typename T> struct is_std_shared_ptr : std::false_type {};
template <typename... ArgsT> struct is_std_shared_ptr <std::shared_ptr<ArgsT...>> : std::true_type {};
template <typename T> constexpr bool is_std_shared_ptr_v = is_std_shared_ptr<T>::value;

template <typename T> struct is_std_duration : std::false_type {};
template <typename... ArgsT> struct is_std_duration<std::chrono::duration<ArgsT...>> : std::true_type {};
template <typename T> constexpr bool is_std_duration_v = is_std_duration<T>::value;

template <typename T> struct is_std_time_point : std::false_type {};
template <typename... ArgsT> struct is_std_time_point<std::chrono::time_point<ArgsT...>> : std::true_type {};
template <typename T> constexpr bool is_std_time_point_v = is_std_time_point<T>::value;

template <typename T> struct is_std_tuple : std::false_type {};
template <typename... ArgsT> struct is_std_tuple<std::tuple<ArgsT...>> : std::true_type {};
template <typename T> constexpr bool is_std_tuple_v = is_std_tuple<T>::value;

//-----------------------------------------------------------------------------
// Type extraction.
//-----------------------------------------------------------------------------

// ExtractType: forward declaration
template <size_t, typename> 
struct ExtractType;

// ExtractType_t: alias interface
template <size_t idx, typename C>
using ExtractType_t = typename ExtractType<idx, C>::type;

// ExtractType: base
template <template <typename...> typename C, typename T, typename... RestT>
struct ExtractType <0, C<T, RestT...>> {
  using type = T;
};

// ExtractType: base
template <typename T>
struct ExtractType <0, T> {
  using type = T;
};

// ExtractType: recursive definition.
template <size_t idx, template <typename...> typename C, typename T, typename... RestT>
struct ExtractType <idx, C<T, RestT...>> : ExtractType<idx-1, C<RestT...>> {
};

// ----------------------------------------------------------------------------
// Size Wrapper
// ----------------------------------------------------------------------------

// Struct: SizeTag
// Class that wraps a given size item which can be customized. 
template <typename T>
class SizeTag {

  public: 
  
    using type = std::conditional_t<std::is_lvalue_reference_v<T>, T, std::decay_t<T>>;
    
    SizeTag(T&& item) : _item(std::forward<T>(item)) {}
    
    SizeTag& operator = (const SizeTag&) = delete;

    inline const T& get() const {return _item;}

    template <typename ArchiverT>
    std::streamsize archive(ArchiverT & ar) { return ar(_item); }

  private:

    type _item;
};

// Function: make_size_tag
template <typename T>
SizeTag<T> make_size_tag(T&& t) {
  return { std::forward<T>(t) };
}

// ----------------------------------------------------------------------------
// Size Wrapper
// ----------------------------------------------------------------------------

// Class: MapItem
template <typename KeyT, typename ValueT>
class MapItem {
  
  public:
  
    using KeyType = std::conditional_t <std::is_lvalue_reference_v<KeyT>, KeyT, std::decay_t<KeyT>>;
    using ValueType = std::conditional_t <std::is_lvalue_reference_v<ValueT>, ValueT, std::decay_t<ValueT>>;

    MapItem(KeyT&& k, ValueT&& v) : _key(std::forward<KeyT>(k)), _value(std::forward<ValueT>(v)) {}
    MapItem& operator = (const MapItem&) = delete;

    inline const KeyT& key() const { return _key; }
    inline const ValueT& value() const { return _value; }

    template <typename ArchiverT>
    std::streamsize archive(ArchiverT & ar) { return ar(_key, _value); }

  private:

    KeyType _key;
    ValueType _value;
};

// Function: make_kv_pair
template <typename KeyT, typename ValueT>
MapItem<KeyT, ValueT> make_kv_pair(KeyT&& k, ValueT&& v) {
  return { std::forward<KeyT>(k), std::forward<ValueT>(v) };
}

// ----------------------------------------------------------------------------

// Class: Ciri
template <typename Device = std::ostream, typename SizeType = std::streamsize>
class Ciri {

  public:
    
    Ciri(Device& device);
    
    template <typename... T>
    SizeType operator()(T&&... items);
  
  private:

    Device& _device;
    
    template <typename T>
    SizeType _ciri(T&&);
};

// Constructor
template <typename Device, typename SizeType>
Ciri<Device, SizeType>::Ciri(Device& device) : _device(device) {
}

// Operator ()
template <typename Device, typename SizeType>
template <typename... T>
SizeType Ciri<Device, SizeType>::operator() (T&&... items) {
  return (_ciri(std::forward<T>(items)) + ...);
}

// Function: _ciri
template <typename Device, typename SizeType>
template <typename T>
SizeType Ciri<Device, SizeType>::_ciri(T&& t) {

  using U = std::decay_t<T>;
  
  // arithmetic data type
  if constexpr(std::is_arithmetic_v<U>) {
    _device.write(reinterpret_cast<const char*>(std::addressof(t)), sizeof(t));
    return sizeof(t);
  }
  // std::basic_string
  else if constexpr(is_std_basic_string_v<U>) {
    auto sz = _ciri(make_size_tag(t.size()));
    _device.write(reinterpret_cast<const char*>(t.data()), t.size()*sizeof(typename U::value_type));
    return sz + t.size()*sizeof(typename U::value_type);
  }
  // std::vector
  else if constexpr(is_std_vector_v<U>) {
    if constexpr (std::is_arithmetic_v<typename U::value_type>) {
      auto sz = _ciri(make_size_tag(t.size()));
      _device.write(reinterpret_cast<const char*>(t.data()), t.size() * sizeof(typename U::value_type));
      return sz + t.size() * sizeof(typename U::value_type);
    } else {
      auto sz = _ciri(make_size_tag(t.size()));
      for(auto&& item : t) {
        sz += _ciri(item);
      }
      return sz;
    }
  }
  // std::deque and std::list
  else if constexpr(is_std_deque_v<U> || is_std_list_v<U>) {
    auto sz = _ciri(make_size_tag(t.size()));
    for(auto&& item : t) {
      sz += _ciri(item);
    }
    return sz;
  }
  // std::forward_list
  else if constexpr(is_std_forward_list_v<U>) {
    auto sz = _ciri(make_size_tag(std::distance(t.begin(), t.end())));
    for(auto&& item : t) {
      sz += _ciri(item);
    }
    return sz;
  }
  // std::map and std::unordered_map
  else if constexpr(is_std_map_v<U> || is_std_unordered_map_v<U>) {
    auto sz = _ciri(make_size_tag(t.size()));
    for(auto&& [k, v] : t) {
      sz += _ciri(make_kv_pair(k, v));
    }
    return sz;
  }
  // std::set and std::unordered_set
  else if constexpr(is_std_set_v<U> || is_std_unordered_set_v<U>) {
    auto sz = _ciri(make_size_tag(t.size()));
    for(auto&& item : t) {
      sz += _ciri(item);
    }
    return sz;
  }
  // enum
  else if constexpr(std::is_enum_v<U>) {
    return _ciri(static_cast<std::underlying_type_t<U>>(t));
  }
  // std::array
  else if constexpr(is_std_array_v<U>) {
    static_assert(std::tuple_size<U>::value > 0, "Array size can't be zero");

    if constexpr(std::is_arithmetic_v<typename U::value_type>) {
      _device.write(reinterpret_cast<const char*>(t.data()), sizeof(t));
      return sizeof(t);
    } 
    else {
      SizeType sz {0};
      for(auto&& item : t) {
        sz += _ciri(item);
      }
      return sz;
    }
  }
  // std::variant
  else if constexpr(is_std_variant_v<U>) {
    return _ciri(t.index()) + 
           std::visit([&] (auto&& arg){ return _ciri(arg);}, t);
  }
  // std::duration
  else if constexpr(is_std_duration_v<U>) {
    return _ciri(t.count());
  }
  // std::time_point
  else if constexpr(is_std_time_point_v<U>) {
    return _ciri(t.time_since_epoch());
  }
  // std::optional
  else if constexpr(is_std_optional_v<U>) {
    if(bool flag = t.has_value(); flag) {
      return _ciri(flag) + _ciri(*t);
    }
    else {
      return _ciri(flag);
    }
  }
  // std::tuple
  else if constexpr(is_std_tuple_v<U>) {
    return std::apply(
      [this] (auto&&... args) {
        return (_ciri(std::forward<decltype(args)>(args)) + ... + 0); 
      },
      std::forward<T>(t)
    );
  }
  // Fall back to user-defined serialization method.
  else {
    return t.archive(*this);
  }
}

// ----------------------------------------------------------------------------

// Class: Iric
template <typename Device = std::ostream, typename SizeType = std::streamsize>
class Iric {

  public:
    
    Iric(Device& device);
    
    template <typename... T>
    SizeType operator()(T&&... items);
  
  private:

    Device& _device;
    
    template <typename T>
    SizeType _iric(T&&);
    
    // Function: _variant_helper
    template <size_t I = 0, typename... ArgsT, std::enable_if_t<I==sizeof...(ArgsT)>* = nullptr>
    SizeType _variant_helper(size_t, std::variant<ArgsT...>&);
    
    // Function: _variant_helper
    template <size_t I = 0, typename... ArgsT, std::enable_if_t<I<sizeof...(ArgsT)>* = nullptr>
    SizeType _variant_helper(size_t, std::variant<ArgsT...>&);
};

// Constructor
template <typename Device, typename SizeType>
Iric<Device, SizeType>::Iric(Device& device) : _device(device) {
}

// Operator ()
template <typename Device, typename SizeType>
template <typename... T>
SizeType Iric<Device, SizeType>::operator() (T&&... items) {
  return (_iric(std::forward<T>(items)) + ...);
}

// Function: _iric
template <typename Device, typename SizeType>
template <typename T>
SizeType Iric<Device, SizeType>::_iric(T&& t) {

  using U = std::decay_t<T>;
  
  // arithmetic data type
  if constexpr(std::is_arithmetic_v<U>) {
    _device.read(reinterpret_cast<char*>(std::addressof(t)), sizeof(t));
    return sizeof(t);
  }
  // std::basic_string
  else if constexpr(is_std_basic_string_v<U>) {
    typename U::size_type num_chars;
    auto sz = _iric(make_size_tag(num_chars));
    t.resize(num_chars);
    _device.read(reinterpret_cast<char*>(t.data()), num_chars*sizeof(typename U::value_type));
    return sz + num_chars*sizeof(typename U::value_type);
  }
  // std::vector
  else if constexpr(is_std_vector_v<U>) {
    typename U::size_type num_data;
    if constexpr(std::is_arithmetic_v<typename U::value_type>) {
      auto sz = _iric(make_size_tag(num_data));
      t.resize(num_data);
      _device.read(reinterpret_cast<char*>(t.data()), num_data * sizeof(typename U::value_type));
      return sz + num_data * sizeof(typename U::value_type);
    } 
    else {
      auto sz = _iric(make_size_tag(num_data));
      t.resize(num_data);
      for(auto && v : t) {
        sz += _iric(v);
      }
      return sz;
    }
  }
  // std::deque, std::list, and std::forward_list
  else if constexpr(is_std_deque_v<U> || is_std_list_v<U> || is_std_forward_list_v<U>) {

    typename U::size_type num_data;
    auto sz = _iric(make_size_tag(num_data));

    t.resize(num_data);
    for(auto && v : t) {
      sz += _iric(v);
    }
    return sz;
  }
  // std::map
  else if constexpr(is_std_map_v<U>) {

    typename U::size_type num_data;
    auto sz = _iric(make_size_tag(num_data));
    
    t.clear();
    auto hint = t.begin();
      
    typename U::key_type k;
    typename U::mapped_type v;

    for(size_t i=0; i<num_data; ++i) {
      sz += _iric(make_kv_pair(k, v));
      hint = t.emplace_hint(hint, std::move(k), std::move(v));
    }
    return sz;
  }
  // std::unordered_map
  else if constexpr(is_std_unordered_map_v<U>) {

    typename U::size_type num_data;
    auto sz = _iric(make_size_tag(num_data));

    t.clear();
    t.reserve(num_data);

    typename U::key_type k;
    typename U::mapped_type v;

    for(size_t i=0; i<num_data; ++i) {
      sz += _iric(make_kv_pair(k, v));
      t.emplace(std::move(k), std::move(v));
    }
    
    return sz;
  }
  // std::set
  else if constexpr(is_std_set_v<U>) {

    typename U::size_type num_data;
    auto sz = _iric(make_size_tag(num_data));

    t.clear();
    auto hint = t.begin();
      
    typename U::key_type k;

    for(size_t i=0; i<num_data; ++i) {   
      sz += _iric(k);
      hint = t.emplace_hint(hint, std::move(k));
    }   
    return sz;
  }
  // std::unordered_set
  else if constexpr(is_std_unordered_set_v<U>) {

    typename U::size_type num_data;
    auto sz = _iric(make_size_tag(num_data));

    t.clear();
    t.reserve(num_data);
      
    typename U::key_type k;

    for(size_t i=0; i<num_data; ++i) {   
      sz += _iric(k);
      t.emplace(std::move(k));
    }   
    return sz;
  }
  // enum
  else if constexpr(std::is_enum_v<U>) {
    std::underlying_type_t<U> k;
    auto sz = _iric(k);
    t = static_cast<U>(k);
    return sz;
  }
  // std::array
  else if constexpr(is_std_array_v<U>) {
    static_assert(std::tuple_size<U>::value > 0, "Array size can't be zero");
      
    if constexpr(std::is_arithmetic_v<typename U::value_type>) {
      _device.read(reinterpret_cast<char*>(t.data()), sizeof(t));
      return sizeof(t);
    } 
    else {
      auto sz = std::streamsize{0};
      for(auto && v : t) {
        sz += _iric(v);
      }
      return sz;
    }
  }
  // std::variant
  else if constexpr(is_std_variant_v<U>) {
    std::decay_t<decltype(t.index())> idx;
    auto s = _iric(idx);
    return s + _variant_helper(idx, t);
  }
  // std::duration
  else if constexpr(is_std_duration_v<U>) {
    typename U::rep count;
    auto s = _iric(count);
    t = U{count};
    return s;
  }
  // std::time_point
  else if constexpr(is_std_time_point_v<U>) {
    typename U::duration elapsed;
    auto s = _iric(elapsed);
    t = U{elapsed};
    return s;
  }
  // std::optional
  else if constexpr(is_std_optional_v<U>) {
    bool has_value;
    auto s = _iric(has_value);
    if(has_value) {
      if(!t) {
        t = typename U::value_type();
      }
      s += _iric(*t);
    }
    else {
      t.reset(); 
    }
    return s;
  }
  // std::tuple
  else if constexpr(is_std_tuple_v<U>) {
    return std::apply(
      [this] (auto&&... args) {
        return (_iric(std::forward<decltype(args)>(args)) + ... + 0); 
      },
      std::forward<T>(t)
    );
  }
  else {
    return t.archive(*this);
  }
}
  
// Function: _variant_helper
template <typename Device, typename SizeType>
template <size_t I, typename... ArgsT, std::enable_if_t<I==sizeof...(ArgsT)>*>
SizeType Iric<Device, SizeType>::_variant_helper(size_t i, std::variant<ArgsT...>& v) {
  return 0;
}

// Function: _variant_helper
template <typename Device, typename SizeType>
template <size_t I, typename... ArgsT, std::enable_if_t<I<sizeof...(ArgsT)>*>
SizeType Iric<Device, SizeType>::_variant_helper(size_t i, std::variant<ArgsT...>& v) {
  if(i == 0) {
    using type = ExtractType_t<I, std::variant<ArgsT...>>;
    if(v.index() != I) {
      static_assert(
        std::is_default_constructible<type>::value, 
        "Failed to archive variant (type should be default constructible T())"
      );
      v = type();
    }
    return _iric(std::get<type>(v));
  }
  return _variant_helper<I+1, ArgsT...>(i-1, v);
}

}; // ned of namespace ciri ---------------------------------------------------





