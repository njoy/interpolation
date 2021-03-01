#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "dimwits.hpp"
#include "interpolation.hpp"

#include "header-utilities/copy.hpp"

using namespace njoy::interpolation;
using namespace dimwits;

SCENARIO("An interpolation table can be constructed with a domain minimum"){
  GIVEN("an x- and y-grid"){

    std::vector< double > xGrid{1.0, 2.0, 3.0};
    std::vector< double > yGrid{3.0, 5.0, 7.0};

    Table< table::Type< LinearLinear,
			table::search::Binary,
			table::discontinuity::TakeLeft,
			std::vector< double >,
			std::vector< double > >,
	   table::domain::min::IsRuntimeConstant >
      myTable( 0.0, njoy::utility::copy(xGrid), njoy::utility::copy(yGrid) );

    THEN("the table can be evaluated in the center interval"){
      REQUIRE( 6.0 == myTable( 2.5 ) );
      REQUIRE( 6.0 == myTable( 2.5, myTable.search() ) );
    }

    THEN("the table will throw for values less than the left limit"){
      REQUIRE_THROWS( myTable( -1.0 ) );
      REQUIRE_THROWS( myTable( -1.0, myTable.search() ) );
    }

    THEN("the table can be integrated in the center interval"){
      REQUIRE( 10. == myTable.integrate(1., 3.) );
      REQUIRE( 10. == myTable.integrate(1., 3., myTable.search()) );
    }

    THEN("the table will throw for integration limits less than the domain min"){
      REQUIRE_THROWS( myTable.integrate(-1., 3.) );
      REQUIRE_THROWS( myTable.integrate(-1., 3., myTable.search()) );
    }
  }
}
