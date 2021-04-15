#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "interpolation.hpp"

#include "range/v3/algorithm/equal.hpp"
#include "range/v3/view/take_exactly.hpp"

#include "header-utilities/copy.hpp"

using namespace njoy::interpolation;

SCENARIO("An variant interpolation table can be constructed"){
  GIVEN("an x- and y-grid"){
    std::vector< double > xGrid{1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
    std::vector< double > yGrid{3.0, 5.0, 7.0, 9.0, 11., 13., 15., 17.};

    GIVEN("a Vector can be constructed"){
      const auto leftSize = 3;
      const auto rightSize = 6;

      auto xLeft = xGrid
	| ranges::views::drop_exactly( 0 )
	| ranges::views::take_exactly( leftSize );

      auto yLeft = yGrid
	| ranges::views::drop_exactly( 0 )
	| ranges::views::take_exactly( leftSize );

      auto xRight = xGrid
	| ranges::views::drop_exactly( leftSize - 1 )
	| ranges::views::take_exactly( rightSize );

      auto yRight = yGrid
	| ranges::views::drop_exactly( leftSize - 1 )
	| ranges::views::take_exactly( rightSize );

      using Component = Table< table::Type< LinearLinear,
					    table::search::Binary,
					    table::discontinuity::TakeLeft,
					    decltype(xLeft), decltype(yLeft) > >;

      const std::vector<Component> core{ Component( njoy::utility::copy(xLeft),
						    njoy::utility::copy(yLeft) ),
                                    	 Component( njoy::utility::copy(xRight),
						    njoy::utility::copy(yRight) ) };

      Table< table::Vector< Component > > vc( njoy::utility::copy( core ) );

      REQUIRE( vc.tableMin() == xGrid.front() );
      REQUIRE( vc.tableMax() == xGrid.back() );
      REQUIRE( vc.domainMin() == -infinity<double>() );
      REQUIRE( vc.domainMax() == infinity<double>() );
      REQUIRE( not vc.specifiesLeftInterval() );
      REQUIRE( not vc.specifiesRightInterval() );
      REQUIRE( not vc.specifiesDomainMin() );
      REQUIRE( not vc.specifiesDomainMax() );

      bool correct;
      correct = ranges::cpp20::equal( vc.x(), xGrid );
      REQUIRE(correct);
      correct = ranges::cpp20::equal( vc.y(), yGrid );
      REQUIRE(correct);

      REQUIRE( core[0](2.5) == vc(2.5) );
      REQUIRE( core[1](6.5) == vc(6.5) );
    }

    SECTION("exceptional behavior"){
      auto leftSize = 4;
      auto rightSize = 4;

      GIVEN("a gap"){
        auto xLeft = xGrid
	  | ranges::views::drop_exactly( 0 )
	  | ranges::views::take_exactly( leftSize );

	auto yLeft = yGrid
	  | ranges::views::drop_exactly( 0 )
	  | ranges::views::take_exactly( leftSize );

	auto xRight = xGrid
	  | ranges::views::drop_exactly( leftSize )
	  | ranges::views::take_exactly( rightSize );

	auto yRight = yGrid
	  | ranges::views::drop_exactly( leftSize )
	  | ranges::views::take_exactly( rightSize );

	using Component = Table< table::Type< LinearLinear,
					      table::search::Binary,
					      table::discontinuity::TakeLeft,
					      decltype(xLeft), decltype(yLeft) > >;

	const std::vector<Component> core{ Component( njoy::utility::copy(xLeft),
						      njoy::utility::copy(yLeft) ),
                                      	   Component( njoy::utility::copy(xRight),
						      njoy::utility::copy(yRight) ) };

	REQUIRE_THROWS
	  ( Table< table::Vector< Component > >( njoy::utility::copy( core ) ) );
      }

      GIVEN("an overlap"){
	auto xLeft = xGrid
	  | ranges::views::drop_exactly( 0 )
	  | ranges::views::take_exactly( leftSize + 1);

	auto yLeft = yGrid
	  | ranges::views::drop_exactly( 0 )
	  | ranges::views::take_exactly( leftSize + 1 );

	auto xRight = xGrid
	  | ranges::views::drop_exactly( leftSize - 1 )
	  | ranges::views::take_exactly( rightSize );

	auto yRight = yGrid
	  | ranges::views::drop_exactly( leftSize - 1 )
	  | ranges::views::take_exactly( rightSize );

	using Component = Table< table::Type< LinearLinear,
					      table::search::Binary,
					      table::discontinuity::TakeLeft,
					      decltype(xLeft), decltype(yLeft) > >;

	const std::vector<Component> core{ Component( njoy::utility::copy(xLeft),
						      njoy::utility::copy(yLeft) ),
                                      	   Component( njoy::utility::copy(xRight),
						      njoy::utility::copy(yRight) ) };

	REQUIRE_THROWS
	  ( Table< table::Vector< Component > >( njoy::utility::copy( core ) ) );
      }
    }
  }
}
