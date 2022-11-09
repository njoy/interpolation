#ifndef NJOY21_INTERPOLATION
#define NJOY21_INTERPOLATION

#include <cmath>
#include <type_traits>

#include <optional>
#include <variant>

#include "Log.hpp"
#include "boost/hana.hpp"
#include "range/v3/iterator/operations.hpp"
#include "range/v3/view/subrange.hpp"
#include "range/v3/algorithm/adjacent_find.hpp"
#include "range/v3/algorithm/find_if.hpp"
#include "range/v3/algorithm/is_sorted.hpp"
#include "range/v3/algorithm/lower_bound.hpp"
#include "range/v3/view/join.hpp"

#include "range/v3/view/concat.hpp"
#include "range/v3/view/drop_exactly.hpp"
#include "range/v3/view/transform.hpp"
#include "header-utilities/void_t.hpp"

namespace njoy {

template< typename Left, typename Right, typename = void >
struct Comparable {
  static constexpr bool value = false;
};

template< typename Left, typename Right >
struct Comparable
< Left, Right,
  utility::void_t< decltype( std::declval<Left>() < std::declval<Right>() ) > > {
  static constexpr bool value = true;
};

namespace interpolation {

namespace hana = boost::hana;

static_assert( std::numeric_limits<float>::is_iec559, "IEEE 754 required" );

template< typename T >
constexpr T infinity(){ return std::numeric_limits<T>::infinity(); }

#include "interpolation/Tags.hpp"
#include "interpolation/Interpolant.hpp"
#include "interpolation/table.hpp"

}
}

#endif
