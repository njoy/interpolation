#include "catch.hpp"
#include "interpolation.hpp"

#include "dimwits.hpp"

using namespace njoy::interpolation;
using namespace dimwits;

SCENARIO("The LinearLogarithmic interpolant computes the correct inversion",
         "[interpolant], [LinearLogarithmic]"){
  std::vector<double> xValues {100, 10, 1, 1E-1, 1E-2};

  auto f1 = []( double x ){ return 10.0 * x; };
  auto f2 = []( double x ){ return 10.0 * std::pow(x,2); };
  auto f3 = []( double x ){ return 5.0 * std::pow(x,2); };
  auto f4 = []( double x ){ return 5.0 * std::pow(x,-1.0); };
  auto f5 = []( double x ){ return 5.0 * std::pow(x,-2.0); };
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
    double y3Left = f3( xLeft ), y3Right = f3( xRight );
    double y4Left = f4( xLeft ), y4Right = f4( xRight );
    double y5Left = f5( xLeft ), y5Right = f5( xRight );
    double xBar = avg( xLeft, xRight );

    REQUIRE( not excessiveError( xBar,
				 LogarithmicLogarithmic::invert
				 ( f1(xBar), xLeft, xRight, y1Left, y1Right ) ) );
    REQUIRE( not excessiveError( xBar,
				 LogarithmicLogarithmic::invert
				 ( f2(xBar), xLeft, xRight, y2Left, y2Right ) ) );
    REQUIRE( not excessiveError( xBar,
				 LogarithmicLogarithmic::invert
				 ( f3(xBar), xLeft, xRight, y3Left, y3Right ) ) );
    REQUIRE( not excessiveError( xBar,
				 LogarithmicLogarithmic::invert
				 ( f4(xBar), xLeft, xRight, y4Left, y4Right ) ) );
    REQUIRE( not excessiveError( xBar,
				 LogarithmicLogarithmic::invert
				 ( f5(xBar), xLeft, xRight, y5Left, y5Right ) ) );


  } while( ++iterator != last );
}

SCENARIO("LogarithmicLogarithmic computes the correct inversion with units",
         "[interpolant], [LogarithmicLogarithmic]"){
  std::vector<double> xValues {100, 10, 1, 1E-1, 1E-2};
  auto f = []( auto x ){ return 5.0 * std::pow(x.value,2) * barn; };
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
    auto yLeft = f( xLeft ), yRight = f( xRight );
    auto xBar = avg( xLeft, xRight );

    REQUIRE( not excessiveError( xRight,
				 LogarithmicLogarithmic::invert
				 ( yRight, xLeft, xRight, yLeft, yRight ) ) );
  } while( ++iterator != last );
}
