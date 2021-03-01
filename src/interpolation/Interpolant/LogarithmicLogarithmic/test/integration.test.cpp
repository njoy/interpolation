#include "catch.hpp"
#include "interpolation.hpp"

#include "dimwits.hpp"

#include "range/v3/view/take.hpp"

using namespace njoy::interpolation;

SCENARIO("The LogarithmicLogarithmic interpolant computes the correct integral",
         "[interpolant], [LogarithmicLogarithmic]"){
  std::vector<double> xValues {100, 10, 1, 1E-1, 1E-2};

  // Function Definitions
  auto f1 = []( double x ){ return 10.0 * x; };
  auto f2 = []( double x ){ return 10.0 * std::pow(x,2); };
  auto f3 = []( double x ){ return 5.0 * std::pow(x,2); };
  auto f4 = []( double x ){ return 5.0 * std::pow(x,-1.0); };
  auto f5 = []( double x ){ return 5.0 * std::pow(x,-2.0); };

  // Integral Definitions
  auto F1 = [](double xLow, double xHi){return 5. * (std::pow(xHi,2.) - std::pow(xLow,2.));};
  auto F2 = [](double xLow, double xHi){return (10./3.)*(std::pow(xHi,3.) - std::pow(xLow,3.));};
  auto F3 = [](double xLow, double xHi){return (5./3.)*(std::pow(xHi,3.) - std::pow(xLow,3.));};
  auto F4 = [](double xLow, double xHi){return 5.*(std::log(xHi) - std::log(xLow));};
  auto F5 = [](double xLow, double xHi){return 5.*((1./xLow) - (1./xHi));};
  
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
    double xLow = xLeft, xHi = xRight;
    
    REQUIRE( not excessiveError( F1(xLow, xBar),
                                 LogarithmicLogarithmic::integrate
                                 ( xLow, xBar, xLeft, xRight, y1Left, y1Right ) ) );
    REQUIRE( not excessiveError( F2(xLow, xBar),
                                 LogarithmicLogarithmic::integrate
                                 ( xLow, xBar, xLeft, xRight, y2Left, y2Right ) ) );
    REQUIRE( not excessiveError( F3(xLow, xBar),
                                 LogarithmicLogarithmic::integrate
                                 ( xLow, xBar, xLeft, xRight, y3Left, y3Right ) ) );
    REQUIRE( not excessiveError( F4(xLow, xBar),
                                 LogarithmicLogarithmic::integrate
                                 ( xLow, xBar, xLeft, xRight, y4Left, y4Right ) ) );
    REQUIRE( not excessiveError( F5(xLow, xBar),
                                 LogarithmicLogarithmic::integrate
                                 ( xLow, xBar, xLeft, xRight, y5Left, y5Right ) ) );
    

    REQUIRE( not excessiveError( F1(xBar, xHi),
                                 LogarithmicLogarithmic::integrate
                                 ( xBar, xHi, xLeft, xRight, y1Left, y1Right ) ) );
    REQUIRE( not excessiveError( F2(xBar, xHi),
                                 LogarithmicLogarithmic::integrate
                                 ( xBar, xHi, xLeft, xRight, y2Left, y2Right ) ) );
    REQUIRE( not excessiveError( F3(xBar, xHi),
                                 LogarithmicLogarithmic::integrate
                                 ( xBar, xHi, xLeft, xRight, y3Left, y3Right ) ) );
    REQUIRE( not excessiveError( F4(xBar, xHi),
                                 LogarithmicLogarithmic::integrate
                                 ( xBar, xHi, xLeft, xRight, y4Left, y4Right ) ) );
    REQUIRE( not excessiveError( F5(xBar, xHi),
                                 LogarithmicLogarithmic::integrate
                                 ( xBar, xHi, xLeft, xRight, y5Left, y5Right ) ) );
  } while( iterator != last );  
}

using namespace dimwits;

SCENARIO("The LogarithmicLogarithmic interpolant computes the correct integral with units",
         "[interpolant], [LogarithmicLogarithmic]"){
  std::vector<double> xValues {100, 10, 1, 1E-1, 1E-2};

  // Function Definitions
  auto f1 = []( auto x ){ return 10.0 * x.value * barns; };
  auto f2 = []( auto x ){ return 10.0 * std::pow(x.value,2) * barns; };
  auto f3 = []( auto x ){ return 5.0 * std::pow(x.value,2) * barns; };
  auto f4 = []( auto x ){ return 5.0 * std::pow(x.value,-1.0) * barns; };
  auto f5 = []( auto x ){ return 5.0 * std::pow(x.value,-2.0) * barns; };

  // Integral Definitions
  auto F1 = [](auto xLow, auto xHi){return 5. * (std::pow(xHi.value,2.) - std::pow(xLow.value,2.)) * barns * electronVolts;};
  auto F2 = [](auto xLow, auto xHi){return (10./3.)*(std::pow(xHi.value,3.) - std::pow(xLow.value,3.)) * barns * electronVolts;};
  auto F3 = [](auto xLow, auto xHi){return (5./3.)*(std::pow(xHi.value,3.) - std::pow(xLow.value,3.)) * barns * electronVolts;};
  auto F4 = [](auto xLow, auto xHi){return 5.*(std::log(xHi.value) - std::log(xLow.value)) * barns * electronVolts;};
  auto F5 = [](auto xLow, auto xHi){return 5.*((1./xLow.value) - (1./xHi.value)) * barns * electronVolts;};
  
  auto avg =
    []( auto xLeft, auto xRight ){ return 0.5 * ( xLeft + xRight ); };

  auto excessiveError =
    []( auto reference, auto trial ){
    auto error = std::abs( (trial - reference).value
                           / ( ( reference.value != 0 ) ? reference.value : 1.0 ) );
    return error > 1E-15;
  };

  auto units = xValues |
    ranges::view::take( xValues.size() - 1 ) |
    ranges::view::transform( []( auto arg ){ return arg * electronVolts; } );
  
  auto iterator = units.begin();
  auto last = units.end();
  do {
    auto xLeft = *iterator, xRight = *( ++iterator );
    auto y1Left = f1( xLeft ), y1Right = f1( xRight );
    auto y2Left = f2( xLeft ), y2Right = f2( xRight );
    auto y3Left = f3( xLeft ), y3Right = f3( xRight );
    auto y4Left = f4( xLeft ), y4Right = f4( xRight );
    auto y5Left = f5( xLeft ), y5Right = f5( xRight );
    auto xBar = avg( xLeft, xRight );
    auto xLow = xLeft, xHi = xRight;
    
    REQUIRE( not excessiveError( F1(xLow, xBar),
                                 LogarithmicLogarithmic::integrate
                                 ( xLow, xBar, xLeft, xRight, y1Left, y1Right ) ) );
    REQUIRE( not excessiveError( F2(xLow, xBar),
                                 LogarithmicLogarithmic::integrate
                                 ( xLow, xBar, xLeft, xRight, y2Left, y2Right ) ) );
    REQUIRE( not excessiveError( F3(xLow, xBar),
                                 LogarithmicLogarithmic::integrate
                                 ( xLow, xBar, xLeft, xRight, y3Left, y3Right ) ) );
    REQUIRE( not excessiveError( F4(xLow, xBar),
                                 LogarithmicLogarithmic::integrate
                                 ( xLow, xBar, xLeft, xRight, y4Left, y4Right ) ) );
    REQUIRE( not excessiveError( F5(xLow, xBar),
                                 LogarithmicLogarithmic::integrate
                                 ( xLow, xBar, xLeft, xRight, y5Left, y5Right ) ) );
    

    REQUIRE( not excessiveError( F1(xBar, xHi),
                                 LogarithmicLogarithmic::integrate
                                 ( xBar, xHi, xLeft, xRight, y1Left, y1Right ) ) );
    REQUIRE( not excessiveError( F2(xBar, xHi),
                                 LogarithmicLogarithmic::integrate
                                 ( xBar, xHi, xLeft, xRight, y2Left, y2Right ) ) );
    REQUIRE( not excessiveError( F3(xBar, xHi),
                                 LogarithmicLogarithmic::integrate
                                 ( xBar, xHi, xLeft, xRight, y3Left, y3Right ) ) );
    REQUIRE( not excessiveError( F4(xBar, xHi),
                                 LogarithmicLogarithmic::integrate
                                 ( xBar, xHi, xLeft, xRight, y4Left, y4Right ) ) );
    REQUIRE( not excessiveError( F5(xBar, xHi),
                                 LogarithmicLogarithmic::integrate
                                 ( xBar, xHi, xLeft, xRight, y5Left, y5Right ) ) );
  } while( iterator != last );  
}