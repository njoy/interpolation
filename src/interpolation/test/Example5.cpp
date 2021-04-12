#include <iostream>
#include <vector>

#include "catch.hpp"
#include "dimwits.hpp"
#include "interpolation.hpp"

using namespace njoy::interpolation;
using namespace dimwits;

TEST_CASE("Example5"){
  std::vector< double > xGrid = { 1.0, 2.0, 3.0 };
  std::vector< double > yGrid = { 3.0, 5.0, 7.0 };

  auto energy = xGrid
    | ranges::views::transform( []( auto&& arg ){ return arg * electronVolts; } );

  auto crossSection = yGrid
    | ranges::views::transform( []( auto&& arg ){ return arg * barns; } );

  auto myTable =
    table::make< LinearLinear >( std::move(energy), std::move(crossSection) );

  REQUIRE( myTable( 2.5 * electronVolts ) == 6.0 * barns );
  REQUIRE( myTable( 2.5E-3 * kilo(electronVolts) ) == 6.0 * barns );
}
