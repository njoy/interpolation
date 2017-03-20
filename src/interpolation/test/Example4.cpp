#include <iostream>
#include <vector>

#include "catch.hpp"
#include "interpolation.hpp"

using namespace njoy::interpolation;

TEST_CASE("Example4"){
  std::vector< double > xGrid = { 1.0, 2.0, 3.0 };
  std::vector< double > yGrid = { 3.0, 5.0, 7.0 };

  auto myTable =
    table::make< LinearLinear,
		 table::search::Hashed<Binary> >( std::move(xGrid),
						  std::move(yGrid) );

  REQUIRE( myTable( 2.5 ) == 6.0 );
}
