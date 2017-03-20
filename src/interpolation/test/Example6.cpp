#include <iostream>
#include <vector>

#include "catch.hpp"
#include "interpolation.hpp"

using namespace njoy::interpolation;

struct Zero {
  static constexpr double value = 0.0;
};

struct Ten {
  static constexpr double value = 10.0;
};

TEST_CASE("Example6"){
  std::vector< double > xGrid = { 1.0, 2.0, 3.0 };
  std::vector< double > yGrid = { 3.0, 5.0, 7.0 };

  /* given the setup code in Example 1 */
  
  auto myTable =
    table::make< LinearLinear,
                 table::domain::min::IsRuntimeConstant,
                 table::domain::max::IsCompiletimeConstant<Ten>,
                 table::left::interval::IsCompiletimeConstant<Zero>,
                 table::right::interval::Throws >( 0.0, // domain min
						   std::move(xGrid),
						   std::move(yGrid) );

  REQUIRE( myTable( 2.5 ) == 6.0 );
  REQUIRE( myTable.domainMin() == 0.0 );
  REQUIRE( myTable.domainMax() == 10.0 );
  REQUIRE( myTable.tableMin() == 1.0 );
  REQUIRE( myTable.tableMax() == 3.0 );

  REQUIRE( myTable( 0.5 ) == 0.0 );
  REQUIRE_THROWS( myTable( -1.0 ) );
  REQUIRE_THROWS( myTable( 3.5 ) );
  REQUIRE_THROWS( myTable( 11.0 ) );
}
