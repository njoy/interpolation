#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "interpolation.hpp"

#include "dimwits.hpp"

using namespace njoy::interpolation;
using namespace dimwits;

SCENARIO("The LinearLinear interpolant computes the correct value",
         "[interpolant], [LinearLinear]"){
  double xLeft = 0.0, xRight = 1.0;
  auto f = []( double x ){ return 10.0 * x + 10; };
  double yLeft = f( xLeft ), yRight = f( xRight );
  
  REQUIRE( yLeft == LinearLinear::apply( xLeft, xLeft, xRight, yLeft, yRight ) );
  REQUIRE( f( 0.5 ) == LinearLinear::apply( 0.5, xLeft, xRight, yLeft, yRight ) );
  REQUIRE( yRight == LinearLinear::apply( xRight, xLeft, xRight, yLeft, yRight ) );
}

SCENARIO("The LinearLinear interpolant computes the correct inversion",
         "[interpolant], [LinearLinear]"){
  double xLeft = 0.0, xRight = 1.0;
  auto f = []( double x ){ return 10.0 * x + 10; };
  double yLeft = f( xLeft ), yRight = f( xRight );
  
  REQUIRE( xLeft == LinearLinear::invert( yLeft, xLeft, xRight, yLeft, yRight ) );
  REQUIRE( 0.5 == LinearLinear::invert( 15., xLeft, xRight, yLeft, yRight ) );
  REQUIRE( xRight == LinearLinear::invert( yRight, xLeft, xRight, yLeft, yRight ) );
}

SCENARIO("The LinearLinear interpolant is compatible with units",
         "[interpolant], [Histogram]"){
  auto xLeft = 0.0 * electronVolts;
  auto xRight = 1.0 * electronVolts;
  auto yLeft = 10.0 * barn;
  auto yRight = 20.0 * barns;
  REQUIRE( 15.0 * barns ==
	   LinearLinear::apply( 0.5 * electronVolts, xLeft, xRight, yLeft, yRight ) );
  REQUIRE( 0.5 * electronVolts ==
	   LinearLinear::invert( 15.0 * barns, xLeft, xRight, yLeft, yRight ) );  
}
