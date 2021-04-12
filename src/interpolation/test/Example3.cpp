#include <iostream>
#include <vector>

#include "catch.hpp"
#include "interpolation.hpp"

using namespace njoy::interpolation;

TEST_CASE("Example3"){
  auto viewOf = []( const auto& range, int left = 0, int right = 0 ){
    return ranges::make_subrange( range.begin() + left,
					range.end() - right );
  };

  std::vector< double > xGrid  = { 1.0, 2.0, 3.0, 4.0,  5.0 };
  std::vector< double > y1Grid = { 3.0, 5.0, 7.0, 9.0, 11.0 };
  auto table1 = table::make< LinearLinear >( viewOf(xGrid), std::move(y1Grid) );

  auto search = table1.cachedSearch();
  REQUIRE( table1( 3.5, search ) == 8.0 ); // new search
  REQUIRE( table1( 3.75, search ) == 8.5 ); // cached

  std::vector< double > y2Grid = { 9.0, 7.0, 3.0, 5.0,  7.0 };
  auto table2 = table::make< LinearLinear >( viewOf(xGrid), std::move(y2Grid) );

  REQUIRE( table2( 3.5, search ) == 4.0 ); // also cached

  std::vector< double > y3Grid = { 7.0, 5.0, 3.0 };
  auto table3 = table::make< LinearLinear >( viewOf(xGrid, 2), std::move(y3Grid) );

  REQUIRE( table3( 3.5, search ) == 6.0 ); // also cached
  REQUIRE( table3( 4.5, search ) == 4.0 ); // new

  REQUIRE( table1( 4.75, search ) == 10.5 ); // also cached
}
