struct Interpolant {
  template< typename Range >
  static void verifyXGridAssumptions( Range&& ){}
  
  template< typename Range >
  static void verifyYGridAssumptions( Range&& ){}

  template< typename Range  >
  static auto findChangeOfSign( Range&& range ){
    const auto& first = range.front();
    const auto zero = 0 * first;
    const auto it = 
      ( first > zero ) ? ranges::find_if( range,
    		                          [&]( auto&& x ){ return x <= zero; } ) :
      ( first < zero ) ? ranges::find_if( range,
    		                          [&]( auto&& x ){ return x >= zero; } ) :
                         ranges::find_if( range,
    					  [&]( auto&& x ){ return x != zero; } );
    return ( it == range.end() ) ? it : ranges::prev( it ); 
  }
};

#include "interpolation/Interpolant/Histogram.hpp"
#include "interpolation/Interpolant/LinearLinear.hpp"
#include "interpolation/Interpolant/LinearLogarithmic.hpp"
#include "interpolation/Interpolant/LogarithmicLinear.hpp"
#include "interpolation/Interpolant/LogarithmicLogarithmic.hpp"
