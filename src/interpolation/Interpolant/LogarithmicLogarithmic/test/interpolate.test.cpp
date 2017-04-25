#include "catch.hpp"
#include "interpolation.hpp"

using namespace njoy::interpolation;

SCENARIO("The LogarithmicLogarithmic interpolant computes the correct value",
         "[interpolant], [LogarithmicLogarithmic]"){
  std::vector<double> xValues {100, 10, 1, 1E-1, 1E-2};

  auto f1 = []( double x ){ return 10.0 * x; };
  auto f2 = []( double x ){ return 10.0 * std::pow(x,2); };
  auto f3 = []( double x ){ return 5.0 * std::pow(x,2); };
  auto f4 = []( double x ){ return 5.0 * std::pow(x,-1.0); };
  auto f5 = []( double x ){ return 5.0 * std::pow(x,-2.0); };
  
  auto avg =
    []( double xLeft, double xRight ){ return 0.5 * ( xLeft + xRight ); };

  auto excessiveError =
    []( double reference, double trial ){
    auto error = std::abs( (trial - reference)
                           / ( ( reference != 0 ) ? reference : 1.0 ) );
    return error > 1E-15;
  };
  
  auto iterator = xValues.begin();
  auto last = std::prev( xValues.end() );
  do {
    double xLeft = *iterator, xRight = *( ++iterator );
    double y1Left = f1( xLeft ), y1Right = f1( xRight );
    double y2Left = f2( xLeft ), y2Right = f2( xRight );
    double y3Left = f3( xLeft ), y3Right = f3( xRight );
    double y4Left = f4( xLeft ), y4Right = f4( xRight );
    double y5Left = f5( xLeft ), y5Right = f5( xRight );
    double xBar = avg( xLeft, xRight );
    
    REQUIRE( not excessiveError( y1Left,
                                 LogarithmicLogarithmic::apply
                                 ( xLeft, xLeft, xRight, y1Left, y1Right ) ) );
    REQUIRE( not excessiveError( y2Left,
                                 LogarithmicLogarithmic::apply
                                 ( xLeft, xLeft, xRight, y2Left, y2Right ) ) );
    REQUIRE( not excessiveError( y3Left,
                                 LogarithmicLogarithmic::apply
                                 ( xLeft, xLeft, xRight, y3Left, y3Right ) ) );
    REQUIRE( not excessiveError( y4Left,
                                 LogarithmicLogarithmic::apply
                                 ( xLeft, xLeft, xRight, y4Left, y4Right ) ) );
    REQUIRE( not excessiveError( y5Left,
                                 LogarithmicLogarithmic::apply
                                 ( xLeft, xLeft, xRight, y5Left, y5Right ) ) );
    

    REQUIRE( not excessiveError( f1( xBar ),
                                 LogarithmicLogarithmic::apply
                                 ( xBar, xLeft, xRight, y1Left, y1Right ) ) );
    REQUIRE( not excessiveError( f2( xBar ),
                                 LogarithmicLogarithmic::apply
                                 ( xBar, xLeft, xRight, y2Left, y2Right ) ) );
    REQUIRE( not excessiveError( f3( xBar ),
                                 LogarithmicLogarithmic::apply
                                 ( xBar, xLeft, xRight, y3Left, y3Right ) ) );
    REQUIRE( not excessiveError( f4( xBar ),
                                 LogarithmicLogarithmic::apply
                                 ( xBar, xLeft, xRight, y4Left, y4Right ) ) );
    REQUIRE( not excessiveError( f5( xBar ),
                                 LogarithmicLogarithmic::apply
                                 ( xBar, xLeft, xRight, y5Left, y5Right ) ) );
  } while( iterator != last );  
}
