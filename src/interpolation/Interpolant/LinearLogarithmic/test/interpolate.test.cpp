#include "catch.hpp"
#include "interpolation.hpp"

#include "dimwits.hpp"

using namespace njoy::interpolation;
using namespace dimwits;

SCENARIO("The LinearLogarithmic interpolant computes the correct value",
         "[interpolant], [LinearLogarithmic]"){
  std::vector<double> xValues{ 1E-2, 1, 10 };
  auto f1 = []( double x ){ return 13.2 * std::log(3.2*x); };
  auto f2 = []( double x ){ return -5.6 * std::log(2.4*x); };

  auto avg =
    []( double xLeft, double xRight ){ return 0.5 * ( xLeft + xRight ); };

  auto excessiveError =
    []( double reference, double trial ){
    auto error = std::abs( (trial - reference)
			   / ( ( reference != 0 ) ? reference : 1.0 ) );
    return error > 1E-15; };
  
  auto iterator = xValues.begin();
  auto last = std::prev( xValues.end() );
  do {
    double xLeft = *iterator, xRight = *( ++iterator );
    double y1Left = f1( xLeft ), y1Right = f1( xRight );
    double y2Left = f2( xLeft ), y2Right = f2( xRight );
    double xBar = avg( xLeft, xRight );
    
    REQUIRE( not excessiveError( y1Left,
                                 LinearLogarithmic::apply
                                 ( xLeft, xLeft, xRight, y1Left, y1Right ) ) );
    REQUIRE( not excessiveError( y2Left,
                                 LinearLogarithmic::apply
                                 ( xLeft, xLeft, xRight, y2Left, y2Right ) ) );

    REQUIRE( not excessiveError( f1( xBar ),
                                 LinearLogarithmic::apply
                                 ( xBar, xLeft, xRight, y1Left, y1Right ) ) );
    REQUIRE( not excessiveError( f2( xBar ),
                                 LinearLogarithmic::apply
                                 ( xBar, xLeft, xRight, y2Left, y2Right ) ) );
  } while( iterator != last );
}

SCENARIO("The LinearLogarithmic interpolant computes is compatible with units",
         "[interpolant], [LinearLogarithmic]"){
  std::vector< double > xValues{ 1E-2, 1, 10 };
  auto f1 = []( auto x ){ return 13.2 * std::log(3.2 * x.value) * barn; };
  auto f2 = []( auto x ){ return -5.6 * std::log(2.4 * x.value) * barn; };

  auto avg = []( auto xLeft, auto xRight ) { return 0.5 * ( xLeft + xRight ); };

  auto excessiveError =
    []( auto reference, auto trial ){
    auto error = std::abs( (trial - reference).value
			   / ( ( reference.value != 0 ) ?
			       reference.value : 1.0 ) );
    return error > 1E-15; };

  auto units = xValues |
    ranges::view::take( xValues.size() - 1 ) |
    ranges::view::transform( []( auto arg ){ return arg * electronVolts; } );

  auto iterator = units.begin();
  auto last = units.end();
  do {
    auto xLeft = *iterator, xRight = *( ++iterator );
    auto y1Left = f1( xLeft ), y1Right = f1( xRight );
    auto y2Left = f2( xLeft ), y2Right = f2( xRight );
    auto xBar = avg( xLeft, xRight );
    
    REQUIRE( not excessiveError( y1Left,
                                 LinearLogarithmic::apply
                                 ( xLeft, xLeft, xRight, y1Left, y1Right ) ) );
    REQUIRE( not excessiveError( y2Left,
                                 LinearLogarithmic::apply
                                 ( xLeft, xLeft, xRight, y2Left, y2Right ) ) );

    REQUIRE( not excessiveError( f1( xBar ),
                                 LinearLogarithmic::apply
                                 ( xBar, xLeft, xRight, y1Left, y1Right ) ) );
    REQUIRE( not excessiveError( f2( xBar ),
                                 LinearLogarithmic::apply
                                 ( xBar, xLeft, xRight, y2Left, y2Right ) ) );
  } while( iterator != last );
}
