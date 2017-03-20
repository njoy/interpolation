#include "catch.hpp"
#include "interpolation.hpp"

#include "dimwits.hpp"

using namespace njoy::interpolation;
using namespace dimwits;

SCENARIO("The LinearLogarithmic interpolant computes the correct inversion",
         "[interpolant], [LinearLogarithmic]"){
  std::vector<double> xValues{ 1E-2, 1, 10 };
  auto f1 = []( auto x ){ return 13.2 * pow(3.14, 0.002*x); };
  auto f2 = []( auto x ){ return -13.2 * pow(3.14, 0.002*x); };
  auto avg = []( double xLeft, double xRight ){ return 0.5 * ( xLeft + xRight ); };

  auto excessiveError =
    []( double reference, double trial ){
    auto error = std::abs( (trial - reference)
			   / ( ( reference != 0 ) ? reference : 1.0 ) );
    return error > 5E-14; };
  
  auto iterator = xValues.begin();
  auto last = std::prev( xValues.end() );
  do {
    double xLeft = *iterator, xRight = *std::next( iterator );
    double y1Left = f1( xLeft ), y1Right = f1( xRight );
    double y2Left = f2( xLeft ), y2Right = f2( xRight );
    double xBar = avg( xLeft, xRight );
    REQUIRE( not excessiveError( xLeft,
				 LogarithmicLinear::invert
				 ( y1Left, xLeft, xRight, y1Left, y1Right ) ) );
    REQUIRE( not excessiveError( xRight,
				 LogarithmicLinear::invert
				 ( y1Right, xLeft, xRight, y1Left, y1Right ) ) );
    REQUIRE( not excessiveError( xBar,
				 LogarithmicLinear::invert
				 ( f1(xBar), xLeft, xRight, y1Left, y1Right ) ) );
    REQUIRE( not excessiveError( xLeft,
				 LogarithmicLinear::invert
				 ( y2Left, xLeft, xRight, y2Left, y2Right ) ) );
    REQUIRE( not excessiveError( xRight,
				 LogarithmicLinear::invert
				 ( y2Right, xLeft, xRight, y2Left, y2Right ) ) );
    REQUIRE( not excessiveError( xBar,
				 LogarithmicLinear::invert
				 ( f2(xBar), xLeft, xRight, y2Left, y2Right ) ) );
  } while( ++iterator != last );
}

SCENARIO("LogarithmicLinear computes the correct inversion with units",
         "[interpolant], [LogarithmicLinear]"){
  std::vector<double> xValues{ 1E-2, 1, 10 };
  auto f1 = []( auto x ){ return 13.2 * pow(3.14, 0.002*x.value) * barn; };
  auto f2 = []( auto x ){ return -13.2 * pow(3.14, 0.002*x.value) * barn; };
  auto avg = []( auto xLeft, auto xRight ){ return 0.5 * ( xLeft + xRight ); };

  auto excessiveError =
    []( auto reference, auto trial ){
    auto error = std::abs( (reference - trial).value
			   / ( ( reference.value * trial.value != 0.0 ) ?
			       reference.value : 1.0 ) );
    return error > 5E-14; };

  auto units = xValues |
    ranges::view::take_exactly( xValues.size() - 1 ) |
    ranges::view::transform( []( auto arg ){ return arg * electronVolts; } );
  
  auto iterator = units.begin();
  auto last = units.end();
  do {
    auto xLeft = *iterator, xRight = *std::next( iterator );
    auto y1Left = f1( xLeft ), y1Right = f1( xRight );
    auto y2Left = f2( xLeft ), y2Right = f2( xRight );
    auto xBar = avg( xLeft, xRight );
    REQUIRE( not excessiveError( xLeft,
				 LogarithmicLinear::invert
				 ( y1Left, xLeft, xRight, y1Left, y1Right ) ) );
    REQUIRE( not excessiveError( xRight,
				 LogarithmicLinear::invert
				 ( y1Right, xLeft, xRight, y1Left, y1Right ) ) );
    REQUIRE( not excessiveError( xBar,
				 LogarithmicLinear::invert
				 ( f1(xBar), xLeft, xRight, y1Left, y1Right ) ) );
    REQUIRE( not excessiveError( xLeft,
				 LogarithmicLinear::invert
				 ( y2Left, xLeft, xRight, y2Left, y2Right ) ) );
    REQUIRE( not excessiveError( xRight,
				 LogarithmicLinear::invert
				 ( y2Right, xLeft, xRight, y2Left, y2Right ) ) );
    REQUIRE( not excessiveError( xBar,
				 LogarithmicLinear::invert
				 ( f2(xBar), xLeft, xRight, y2Left, y2Right ) ) );
  } while( ++iterator != last );
}
