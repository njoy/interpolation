#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "interpolation.hpp"

#include "header-utilities/copy.hpp"

using namespace njoy::interpolation;

struct Zero {
  static constexpr double value = 0.0;
};

struct Ten {
  static constexpr double value = 10.0;
};

SCENARIO("table make factory function"){
  GIVEN("a grid of x- and y-values"){
    std::vector< double > xGrid{1.0, 2.0, 3.0};
    std::vector< double > yGrid{3.0, 5.0, 7.0};

    Table< table::Type< LinearLinear,
			table::search::Binary,
			table::discontinuity::TakeLeft,
			std::vector< double >,
			std::vector< double > > >
      reference( njoy::utility::copy(xGrid), njoy::utility::copy(yGrid) );


    THEN("table::make can infer the grid types"){
      auto trial =
	table::make< LinearLinear,
		     table::search::Binary,
		     table::discontinuity::TakeLeft >( njoy::utility::copy(xGrid),
						       njoy::utility::copy(yGrid) );

      bool correct = std::is_same< decltype(reference), decltype(trial) >::value;
      REQUIRE( correct );
    }

    THEN("table::make can apply defaults"){
      bool correct;

      auto trial0 = table::make< LinearLinear >( njoy::utility::copy(xGrid),
						 njoy::utility::copy(yGrid) );

      correct = std::is_same< decltype(reference), decltype(trial0) >::value;
      REQUIRE( correct );

      auto trial1 =
	table::make< LinearLinear,
		     table::search::Binary >( njoy::utility::copy(xGrid),
					      njoy::utility::copy(yGrid) );

      correct = std::is_same< decltype(reference), decltype(trial1) >::value;
      REQUIRE( correct );

      auto trial2 =
	table::make< LinearLinear,
		     table::discontinuity::TakeLeft >( njoy::utility::copy(xGrid),
						       njoy::utility::copy(yGrid) );

      correct = std::is_same< decltype(reference), decltype(trial2) >::value;
      REQUIRE( correct );
    }

    THEN("table::make can apply decorators"){
      Table< table::Type< LinearLinear,
			  table::search::Binary,
			  table::discontinuity::TakeLeft,
			  std::vector< double >,
			  std::vector< double > >,
	     table::domain::min::IsCompiletimeConstant<Zero>,
	     table::domain::max::IsCompiletimeConstant<Ten>,
	     table::left::interval::IsCompiletimeConstant<Zero>,
	     table::right::interval::IsCompiletimeConstant<Ten> >
	reference( njoy::utility::copy(xGrid), njoy::utility::copy(yGrid) );

      auto trial = table::make< LinearLinear,
				table::search::Binary,
				table::discontinuity::TakeLeft,
				table::domain::min::IsCompiletimeConstant<Zero>,
				table::domain::max::IsCompiletimeConstant<Ten>,
				table::left::interval::IsCompiletimeConstant<Zero>,
				table::right::interval::IsCompiletimeConstant<Ten> >
	( njoy::utility::copy(xGrid), njoy::utility::copy(yGrid) );

      bool correct = std::is_same< decltype(reference), decltype(trial) >::value;
      REQUIRE( correct );
    }
  }
}
