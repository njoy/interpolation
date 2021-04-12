#include "catch.hpp"
#include "interpolation.hpp"

#include "dimwits.hpp"

#include "range/v3/view/take.hpp"

using namespace njoy::interpolation;
using namespace dimwits;

SCENARIO("The LinearLogarithmic interpolant computes the correct inversion",
         "[interpolant], [LinearLogarithmic]"){
  std::vector<double> xValues{ 1E-2, 1, 10 };
  auto f1 = []( double x ){ return 13.2 * std::log(3.2*x); };
  auto f2 = []( double x ){ return -5.6 * std::log(2.4*x); };
  auto avg = []( double xLeft, double xRight ){ return 0.5 * ( xLeft + xRight ); };

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
    REQUIRE( not excessiveError( xLeft,
				 LinearLogarithmic::invert
				 ( y1Left, xLeft, xRight, y1Left, y1Right ) ) );
    REQUIRE( not excessiveError( xRight,
				 LinearLogarithmic::invert
				 ( y1Right, xLeft, xRight, y1Left, y1Right ) ) );
    REQUIRE( not excessiveError( xBar,
				 LinearLogarithmic::invert
				 ( f1(xBar), xLeft, xRight, y1Left, y1Right ) ) );
    REQUIRE( not excessiveError( xLeft,
				 LinearLogarithmic::invert
				 ( y2Left, xLeft, xRight, y2Left, y2Right ) ) );
    REQUIRE( not excessiveError( xRight,
				 LinearLogarithmic::invert
				 ( y2Right, xLeft, xRight, y2Left, y2Right ) ) );
    REQUIRE( not excessiveError( xBar,
				 LinearLogarithmic::invert
				 ( f2(xBar), xLeft, xRight, y2Left, y2Right ) ) );
  } while( iterator != last );
}

SCENARIO("LinearLogarithmic computes the correct inversion with units",
         "[interpolant], [LinearLogarithmic]"){
  std::vector<double> xValues{ 1E-2, 1, 10 };

  auto f1 = []( auto x ){ return 13.2 * std::log(3.2 * x.value) * barn; };
  auto f2 = []( auto x ){ return -5.6 * std::log(2.4 * x.value) * barn; };
  auto avg = []( auto xLeft, auto xRight ){ return 0.5 * ( xLeft + xRight ); };

  auto excessiveError =
    []( auto reference, auto trial ){
    auto error = std::abs( (trial - reference).value
			   / ( ( reference.value != 0 ) ?
			       reference.value : 1.0 ) );
    return error > 1E-15; };

  auto units = xValues |
    ranges::cpp20::views::take( xValues.size() - 1 ) |
    ranges::views::transform( []( auto arg ){ return arg * electronVolts; } );

  auto iterator = units.begin();
  auto last = units.end();
  do {
    auto xLeft = *iterator, xRight = *( ++iterator );
    auto y1Left = f1( xLeft ), y1Right = f1( xRight );
    auto y2Left = f2( xLeft ), y2Right = f2( xRight );
    auto xBar = avg( xLeft, xRight );
    REQUIRE( not excessiveError( xLeft,
				 LinearLogarithmic::invert
				 ( y1Left, xLeft, xRight, y1Left, y1Right ) ) );
    REQUIRE( not excessiveError( xRight,
				 LinearLogarithmic::invert
				 ( y1Right, xLeft, xRight, y1Left, y1Right ) ) );
    REQUIRE( not excessiveError( xBar,
				 LinearLogarithmic::invert
				 ( f1(xBar), xLeft, xRight, y1Left, y1Right ) ) );
    REQUIRE( not excessiveError( xLeft,
				 LinearLogarithmic::invert
				 ( y2Left, xLeft, xRight, y2Left, y2Right ) ) );
    REQUIRE( not excessiveError( xRight,
				 LinearLogarithmic::invert
				 ( y2Right, xLeft, xRight, y2Left, y2Right ) ) );
    REQUIRE( not excessiveError( xBar,
				 LinearLogarithmic::invert
				 ( f2(xBar), xLeft, xRight, y2Left, y2Right ) ) );
  } while( iterator != last );
}
