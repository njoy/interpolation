#include <iostream>
#include <vector>

#include "catch.hpp"
#include "interpolation.hpp"

using namespace njoy::interpolation;

TEST_CASE("Example2"){
  std::vector< double > xGrid = { 1.0, 2.0, 3.0 };
  std::vector< double > yGrid = { 3.0, 5.0, 7.0 };

  /* The `make` helper provides default for common choices
   * + when a search policy is unspecified, binary search is used
   * + when a discontinuity policy is unspecified, left preference is used
   */
  auto myTable =
    table::make< LinearLinear >( std::move(xGrid), std::move(yGrid) );
  
  REQUIRE( myTable( 2.5 ) == 6.0 );
}
