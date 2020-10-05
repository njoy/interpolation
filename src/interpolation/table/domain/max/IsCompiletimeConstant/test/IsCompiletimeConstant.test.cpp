#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "dimwits.hpp"
#include "interpolation.hpp"

#include "header-utilities/copy.hpp"

using namespace njoy::interpolation;
using namespace dimwits;

struct Four { static constexpr double value = 4.0; };

SCENARIO("An interpolation table can be constructed with a domain maximum"){
  GIVEN("an x- and y-grid"){

    std::vector< double > xGrid{1.0, 2.0, 3.0};
    std::vector< double > yGrid{3.0, 5.0, 7.0};

    Table< table::Type< LinearLinear,
			table::search::Binary,
			table::discontinuity::TakeLeft,
			std::vector< double >,
			std::vector< double > >,
	   table::domain::max::IsCompiletimeConstant<Four> >
      myTable( njoy::utility::copy(xGrid), njoy::utility::copy(yGrid) );

    THEN("the table can be evaluated in the center interval"){
      REQUIRE( 4.0 == myTable.domainMax() );
    }

    THEN("the table can be evaluated in the center interval"){
      REQUIRE( 6.0 == myTable( 2.5 ) );
    }

    THEN("the table will throw for values grater than the domain max"){
      REQUIRE_THROWS( myTable( 5.0 ) );
    }
  }
}
