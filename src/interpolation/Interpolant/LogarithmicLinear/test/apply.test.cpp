#include "catch.hpp"
#include "interpolation.hpp"

#include "dimwits.hpp"

#include "range/v3/view/take.hpp"

using namespace njoy::interpolation;

SCENARIO("The LogarithmicLinear interpolant computes the correct value",
         "[interpolant], [LogarithmicLinear]"){
  std::vector<double> xValues {10, 1, 1E-1};
  auto f1 = []( double x ){ return 13.2 * pow(3.14, 0.002*x); };
  auto f2 = []( double x ){ return -13.2 * pow(3.14, 0.002*x); };
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

    REQUIRE( not excessiveError( y1Left,
                                 LogarithmicLinear::apply
                                 ( xLeft, xLeft, xRight, y1Left, y1Right ) ) );
    REQUIRE( not excessiveError( y2Left,
                                 LogarithmicLinear::apply
                                 ( xLeft, xLeft, xRight, y2Left, y2Right ) ) );

    REQUIRE( not excessiveError( f1( xBar ),
                                 LogarithmicLinear::apply
                                 ( xBar, xLeft, xRight, y1Left, y1Right ) ) );
    REQUIRE( not excessiveError( f2( xBar ),
                                 LogarithmicLinear::apply
                                 ( xBar, xLeft, xRight, y2Left, y2Right ) ) );
  } while( iterator != last );
}

using namespace dimwits;

SCENARIO("The LogarithmicLinear interpolant computes is compatible with units",
         "[interpolant], [LinearLogarithmic]"){
  std::vector< double > xValues{ 1E-2, 1, 10 };
  auto f1 = []( auto x ){ return 13.2 * pow(3.14, 0.002*x.value) * barn; };
  auto f2 = []( auto x ){ return -13.2 * pow(3.14, 0.002*x.value) * barn; };
  auto avg = []( auto xLeft, auto xRight ) { return 0.5 * ( xLeft + xRight ); };

  auto excessiveError =
    []( auto reference, auto trial ){
    auto error = std::abs( (reference - trial).value
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
                                 LogarithmicLinear::apply
                                 ( xLeft, xLeft, xRight, y1Left, y1Right ) ) );
    REQUIRE( not excessiveError( y2Left,
                                 LogarithmicLinear::apply
                                 ( xLeft, xLeft, xRight, y2Left, y2Right ) ) );

    REQUIRE( not excessiveError( f1( xBar ),
                                 LogarithmicLinear::apply
                                 ( xBar, xLeft, xRight, y1Left, y1Right ) ) );
    REQUIRE( not excessiveError( f2( xBar ),
                                 LogarithmicLinear::apply
                                 ( xBar, xLeft, xRight, y2Left, y2Right ) ) );
  } while( iterator != last );
}
