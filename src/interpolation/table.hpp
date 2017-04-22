template< typename... Args >
class Table;

namespace table {

struct Tag{};

template< typename T, typename = void >
struct HasSearch{
  static constexpr bool value = false;
};

template< typename T >
struct HasSearch< T, utility::void_t< decltype( std::declval<T>().search() ) > >{
  static constexpr bool value = true;
};

template< typename T >
constexpr bool hasSearch = HasSearch<T>::value;

template< typename... Args > class Make;
#include "interpolation/table/Implementation.hpp"

#include "interpolation/table/search.hpp"
#include "interpolation/table/discontinuity.hpp"
#include "interpolation/table/Type.hpp"

#include "interpolation/table/domain.hpp"
#include "interpolation/table/left.hpp"
#include "interpolation/table/right.hpp"
#include "interpolation/table/transform.hpp"

#include "interpolation/table/Variant.hpp"
#include "interpolation/table/Vector.hpp"

template< typename... Args >
constexpr Make< Args... > make{};

}

