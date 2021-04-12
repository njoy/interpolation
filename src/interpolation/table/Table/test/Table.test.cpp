#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "dimwits.hpp"
#include "interpolation.hpp"

#include "range/v3/algorithm/equal.hpp"

#include "header-utilities/copy.hpp"

using namespace njoy::interpolation;
using namespace dimwits;

template< typename Unit >
struct ApplyUnit{
  template< typename Magnitude >
  constexpr auto
  operator()( Magnitude&& magnitude ) const {
    Quantity< Unit, std::decay_t<Magnitude> > q;
    q.value = magnitude;
    return q;
  }
};

template< typename Unit >
constexpr auto applyUnit = ApplyUnit<Unit>{};

struct Zero {
  static constexpr double value = 0.0;
};

struct Ten {
  static constexpr double value = 10.0;
};

SCENARIO("An interpolation table can be constructed"){
  GIVEN("an x- and y-grid"){
    std::vector< double > xGrid{1.0, 2.0, 3.0};
    std::vector< double > yGrid{3.0, 5.0, 7.0};

    THEN("interpolation tables can be constructed which owns each component"){

      Table< table::Type< LinearLinear,
			  table::search::Binary,
			  table::discontinuity::TakeLeft,
			  std::vector< double >,
			  std::vector< double > >,
	     table::domain::min::IsCompiletimeConstant<Zero>,
	     table::domain::max::IsCompiletimeConstant<Ten>,
	     table::left::interval::IsCompiletimeConstant<Zero>,
	     table::right::interval::IsCompiletimeConstant<Ten> >
          myTable( njoy::utility::copy(xGrid), njoy::utility::copy(yGrid) );

      THEN("the table can be evaluated"){
	REQUIRE( 6.0 == myTable( 2.5 ) );
      }

      THEN("the table can be evaluated in the left interval"){
	REQUIRE( 0.0 == myTable( 0.5 ) );
      }

      THEN("the table can be evaluated in the right interval"){
	REQUIRE( 10.0 == myTable( 8.0 ) );
      }

      THEN("the table throws when evaluated left of the left limit"){
	REQUIRE_THROWS( myTable( -1.0 ) );
      }

      THEN("the table throws when evaluated right of the right limit"){
	REQUIRE_THROWS( myTable( 11.0 ) );
      }

      THEN("the table can provide x-values"){
	REQUIRE( ranges::cpp20::equal( myTable.x(), xGrid ) );
      }

      THEN("the table can provide y-values"){
	REQUIRE( ranges::cpp20::equal( myTable.y(), yGrid ) );
      }

      THEN("the table can provide interval limits"){
	REQUIRE(  myTable.tableMin() ==  xGrid.front() );
	REQUIRE(  myTable.tableMax() ==  xGrid.back() );
      }

      THEN("the domain left limit is negative infinity"){
	REQUIRE( myTable.domainMin() == 0.0 );
      }

      THEN("the domain right limit is infinity"){
	REQUIRE( myTable.domainMax() == 10.0 );
      }
    }
  }
}
