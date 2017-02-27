#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "interpolation.hpp"

using namespace njoy::interpolation;

SCENARIO("The Histogram interpolant computes the correct value",
         "[interpolant], [Histogram]"){
  double xLeft = 0.0, xRight = 1.0, yLeft = 10.0, yRight = 20.0;
  REQUIRE( yLeft == Histogram::apply( 0.0, xLeft, xRight, yLeft, yRight ) );
  REQUIRE( yLeft == Histogram::apply( 0.5, xLeft, xRight, yLeft, yRight ) );
}

SCENARIO("The Histogram rejects grids with redundant x-values",
         "[interpolant], [Histogram]"){
  std::vector< double > xGrid{ 0.0, 1.0, 2.0, 3.0 };
  REQUIRE_NOTHROW( Histogram::verifyXGridAssumptions( xGrid ) );
  xGrid.push_back( xGrid.back() ); 
  REQUIRE_THROWS( Histogram::verifyXGridAssumptions( xGrid ) );
}
