#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "interpolation.hpp"

#include "range/v3/algorithm/equal.hpp"

#include "header-utilities/copy.hpp"

using namespace njoy::interpolation;

SCENARIO("An variant interpolation table can be constructed"){
  GIVEN("an x- and y-grid"){
    std::vector< double > xGrid{1.0, 2.0, 3.0};
    std::vector< double > yGrid{3.0, 5.0, 7.0};

    GIVEN("Varaints sharing an underlying data type"){
      using Law2 =
	Table< table::Type< LinearLinear,
			    table::search::Binary,
			    table::discontinuity::TakeLeft,
			    std::vector< double >, std::vector< double > > >;

      using Law5 =
	Table< table::Type< LogarithmicLogarithmic,
			    table::search::Binary,
			    table::discontinuity::TakeLeft,
			    std::vector< double >, std::vector< double > > >;

      Table< table::Variant< Law2, Law5 > >
	myTable( Law2( njoy::utility::copy(xGrid), njoy::utility::copy(yGrid) ) );

      bool correct;

      correct = ranges::equal( myTable.x(), xGrid );
      REQUIRE( correct );

      correct = ranges::equal( myTable.y(), yGrid );
      REQUIRE( correct );

      REQUIRE( 6.0 == myTable(2.5) );
      REQUIRE( 10. == myTable.integrate(1., 3.) );
      REQUIRE( -10. == myTable.integrate(3., 1.) );
      REQUIRE( -infinity<double>() == myTable.domainMin() );
      REQUIRE( infinity<double>() == myTable.domainMax() );
      REQUIRE( xGrid.front() == myTable.tableMin() );
      REQUIRE( xGrid.back() == myTable.tableMax() );
      REQUIRE( false == myTable.specifiesDomainMin() );
      REQUIRE( false == myTable.specifiesDomainMax() );
      REQUIRE( false == myTable.specifiesLeftInterval() );
      REQUIRE( false == myTable.specifiesRightInterval() );

      Law2 law2( njoy::utility::copy(xGrid), njoy::utility::copy(yGrid) );
      Law5 law5( njoy::utility::copy(xGrid), njoy::utility::copy(yGrid) );

      Table< table::Variant< Law2, Law5 > > t1 = law2;
      Table< table::Variant< Law2, Law5 > > t2 = law5;
    }

    GIVEN("Variants with distinct underlying data types"){
      using Law2 = Table< table::Type< LinearLinear,
    				       table::search::Binary,
    				       table::discontinuity::TakeLeft,
    				       std::vector< double >,
    				       std::vector< double > > >;

      using Law5 = Table< table::Type< LogarithmicLogarithmic,
    				       table::search::Binary,
    				       table::discontinuity::TakeLeft,
    				       std::array< double, 3 >,
    				       std::array< double, 3 > > >;

      Table< table::Variant< Law2, Law5 > >
    	myTable( Law2( njoy::utility::copy(xGrid),
    		       njoy::utility::copy(yGrid) ) );

      bool correct;

      correct = ranges::equal( myTable.x(), xGrid );
      REQUIRE( correct );

      correct = ranges::equal( myTable.y(), yGrid );
      REQUIRE( correct );

      REQUIRE( 6.0 == myTable( 2.5 ) );
      REQUIRE( 6.0 == myTable( 2.5, myTable.search() ) );

      REQUIRE( -infinity<double>() == myTable.domainMin() );
      REQUIRE( infinity<double>() == myTable.domainMax() );

    }
  }
}
