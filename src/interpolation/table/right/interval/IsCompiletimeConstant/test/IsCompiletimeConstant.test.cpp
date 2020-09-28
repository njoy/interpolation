#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "dimwits.hpp"
#include "interpolation.hpp"

#include "header-utilities/copy.hpp"

using namespace njoy::interpolation;
using namespace dimwits;

struct Ten { static constexpr double value = 10.0; };

SCENARIO("An interpolation table can be constructed"
	 " such that the left interval throws"){
  GIVEN("an x- and y-grid"){

    std::vector< double > xGrid{1.0, 2.0, 3.0};
    std::vector< double > yGrid{3.0, 5.0, 7.0};

    Table< table::Type< LinearLinear,
			table::search::Binary,
			table::discontinuity::TakeLeft,
			std::vector< double >,
			std::vector< double > >,
	   table::right::interval::IsCompiletimeConstant<Ten> >
      myTable( njoy::utility::copy(xGrid), njoy::utility::copy(yGrid) );

    THEN("the table can be evaluated in the center interval"){
      REQUIRE( 6.0 == myTable( 2.5, myTable.search() ) );
    }

    THEN("the table will throw in the right interval"){
      REQUIRE( 10.0 == myTable( 4.0, myTable.search() ) );
    }
  }
}
