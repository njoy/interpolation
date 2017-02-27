struct Interpolant {
  template< typename Range >
  static void verifyXGridAssumptions( Range&& ){}
  
  template< typename Range >
  static void verifyYGridAssumptions( Range&& ){}

  template< typename Range  >
  auto findChangeOfSign( Range&& range ){
    const auto& first = range.front();
    const auto zero = 0 * first;
    const auto it = 
      ( first > zero ) ? ranges::find_if( range,
    		                          []( double x ){ return x <= 0.0; } ) :
      ( first < zero ) ? ranges::find_if( range,
    		                          []( double x ){ return x >= 0.0; } ) :
                         ranges::find_if( range,
    					  []( double x ){ return x != 0.0; } );
    return ( it == range.end() ) ? it : std::prev( it ); 
  }
};

#include "interpolation/Interpolant/Histogram.hpp"
//#include "interpolation/Interpolant/LinearLinear.hpp"
//#include "interpolation/Interpolant/LinearLogarithmic.hpp"
//#include "interpolation/Interpolant/LogarithmicLinear.hpp"
//#include "interpolation/Interpolant/LogarithmicLogarithmic.hpp"
