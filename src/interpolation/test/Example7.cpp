#include <iostream>
#include <vector>

#include "catch.hpp"
#include "interpolation.hpp"

using namespace njoy::interpolation;

TEST_CASE("Example7"){
  std::vector< double > xGrid = { 1.0, 2.0, 3.0 };
  std::vector< double > yGrid = { 3.0, 5.0, 7.0 };

  using Law2 =
    decltype( table::make< LinearLinear >( std::move(xGrid),
					   std::move(yGrid) ) );

  using Law5 =
    decltype( table::make< LogarithmicLogarithmic >( std::move(xGrid),
                                                     std::move(yGrid) ) );

  Table< table::Variant< Law2, Law5 > > myTable( Law2( std::move(xGrid),
                                                       std::move(yGrid) ) );

  REQUIRE( myTable(2.5) == 6.0 );
}
