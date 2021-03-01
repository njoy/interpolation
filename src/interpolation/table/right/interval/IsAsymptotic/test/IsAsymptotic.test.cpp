#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "interpolation.hpp"

#include "range/v3/view/take_exactly.hpp"

#include "header-utilities/copy.hpp"

using namespace njoy::interpolation;

SCENARIO("An asymptotic right interval can be applied to a table"){
  GIVEN("an x- and y-grid"){
    std::vector< double > xGrid{1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
    std::vector< double > yGrid{3.0, 5.0, 7.0, 9.0, 11., 13., 15., 17.};

    WHEN("constructing a Table with the search method"){
      Table< table::Type< LinearLinear,
			  table::search::Binary,
			  table::discontinuity::TakeLeft,
			  std::vector< double >,
			  std::vector< double > >,
	     table::right::interval::IsAsymptotic >
	myTable( njoy::utility::copy(xGrid), njoy::utility::copy(yGrid) );

      REQUIRE( myTable(3.5) == 8.0 );
      REQUIRE( myTable(9.0) == 17.0 );

      REQUIRE( myTable.integrate(1., 8.) == 70.);
      REQUIRE( myTable.integrate(1., 9.) == 87.);
    }

    WHEN("constructing a Table without the search method"){
      const auto leftSize = 3;
      const auto rightSize = 6;

      auto xLeft = xGrid
	| ranges::view::drop_exactly( 0 )
	| ranges::view::take_exactly( leftSize );

      auto yLeft = yGrid
	| ranges::view::drop_exactly( 0 )
	| ranges::view::take_exactly( leftSize );

      auto xRight = xGrid
	| ranges::view::drop_exactly( leftSize - 1 )
	| ranges::view::take_exactly( rightSize );

      auto yRight = yGrid
	| ranges::view::drop_exactly( leftSize - 1 )
	| ranges::view::take_exactly( rightSize );

      using Component = Table< table::Type< LinearLinear,
					    table::search::Binary,
					    table::discontinuity::TakeLeft,
					    decltype(xLeft), decltype(yLeft) > >;

      const std::vector<Component> core{ Component( njoy::utility::copy(xLeft),
						    njoy::utility::copy(yLeft) ),
                                    	 Component( njoy::utility::copy(xRight),
						    njoy::utility::copy(yRight) ) };

      Table< table::Vector< Component >,
	     table::right::interval::IsAsymptotic >
	myTable( njoy::utility::copy( core ) );

      REQUIRE( myTable(3.5) == 8.0 );
      REQUIRE( myTable(9.0) == 17.0 );

      REQUIRE( myTable.integrate(1., 8.) == 70.);
      REQUIRE( myTable.integrate(1., 9.) == 87.);
    }
  }
}
