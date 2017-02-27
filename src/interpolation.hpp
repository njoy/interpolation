#ifndef NJOY21_INTERPOLATION
#define NJOY21_INTERPOLATION

#include <cmath>
#include <type_traits>

#ifdef USING_CPP17
  #include <optional>
  #include <variant>
#else 
  #ifdef __GNUC__
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-parameter"
  #endif
  #include <variant>	
  #ifdef __GNUC__
    #pragma GCC diagnostic pop
  #endif
  #include <experimental/optional>

namespace std {
    using namespace std::experimental;
  }
#endif

#include "Log.hpp"
#include "range/v3/all.hpp"

namespace njoy {
namespace interpolation {

static_assert( std::numeric_limits<float>::is_iec559, "IEEE 754 required" );

#include "interpolation/Interpolant.hpp"
//#include "interpolation/table.hpp"

}

#endif
