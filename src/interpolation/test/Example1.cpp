#include <iostream>
#include <vector>

#include "catch.hpp"
#include "interpolation.hpp"

using namespace njoy::interpolation;

TEST_CASE("Example1"){
  std::vector< double > xGrid = { 1.0, 2.0, 3.0 };
  std::vector< double > yGrid = { 3.0, 5.0, 7.0 };

  /* A linear-linear interpolation table using
   * + double for the x-value datatype
   * + vector<double> for the x-range
   * + double for the y-value datatype
   * + vector<double> for the y-range
   * + binary search to determine the appropriate bin 
   * + prefering the left argument given coincident points
   */
  auto myTable =
    table::make< LinearLinear,
                 table::search::Binary,
                 table::discontinuity::TakeLeft >( std::move(xGrid),
                                                   std::move(yGrid) );

  // Interpolation tables are function objects and are callable
  REQUIRE( myTable( 2.5 ) == 6.0 );
  REQUIRE( myTable.domainMin() == -infinity<double>() );
  REQUIRE( myTable.domainMax() == infinity<double>() );
  REQUIRE( myTable.tableMin() == 1.0 );
  REQUIRE( myTable.tableMax() == 3.0 );
}
