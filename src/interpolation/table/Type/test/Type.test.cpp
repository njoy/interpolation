#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "dimwits.hpp"
#include "interpolation.hpp"

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

SCENARIO("An interpolation table can be constructed"){
  GIVEN("an x- and y-grid"){
    std::vector< double > xGrid{1.0, 2.0, 2.0, 3.0};
    std::vector< double > yGrid{3.0, 5.0, 6.0, 7.0};

    THEN("interpolation tables can be constructed which owns each component"){

      Table< table::Type< LinearLinear,
			  table::search::Binary,
			  table::discontinuity::TakeLeft,
			  std::vector< double >, std::vector< double > > >
          myTable( njoy::utility::copy(xGrid), njoy::utility::copy(yGrid) );

      THEN("the table can be evaluated"){
	REQUIRE( 3.0 == myTable( 1.0 ) );
	REQUIRE( 7.0 == myTable( 3.0 ) );
	
	REQUIRE( 6.5 == myTable( 2.5 ) );
	REQUIRE( 6.5 == myTable( 2.5, myTable.search() ) );
      }

      THEN("the table handles discontinuities"){
	REQUIRE( 5.0 == myTable( 2.0, myTable.search() ) );
      }

      THEN("the table can provide x-values"){
	REQUIRE( ranges::equal( myTable.x(), xGrid ) );
      }

      THEN("the table can provide y-values"){
	REQUIRE( ranges::equal( myTable.y(), yGrid ) );
      }

      THEN("the table can provide interval limits"){
	REQUIRE(  myTable.tableMin() ==  xGrid.front() );
	REQUIRE(  myTable.tableMax() ==  xGrid.back() );
      }
      
      THEN("the domain left limit is negative infinity"){
	REQUIRE( myTable.domainMin() == -infinity<double>() );
      }

      THEN("the domain right limit is infinity"){
	REQUIRE( myTable.domainMax() == infinity<double>() );
      }
    }

    THEN("interpolation tables can be constructed which refs each component"){
      auto x = xGrid | ranges::view::all;
      auto y = yGrid | ranges::view::all;
      
      Table< table::Type< LinearLinear,
			  table::search::Binary,
			  table::discontinuity::TakeLeft,
			  decltype(x), decltype(y) > >
	myTable( std::move(x), std::move(y) );

      THEN("the table can be evaluated"){
	REQUIRE( 6.5 == myTable( 2.5 ) );
	REQUIRE( 6.5 == myTable( 2.5, myTable.search() ) );
      }

      THEN("the table can provide x-values"){
	REQUIRE( ranges::equal( myTable.x(), xGrid ) );
      }

      THEN("the table can provide y-values"){
	REQUIRE( ranges::equal( myTable.y(), yGrid ) );
      }      
    }

    THEN("interpolation tables can hash query values"){
      Table< table::Type< LinearLinear,
			  table::search::Hashed<Binary>,
			  table::discontinuity::TakeLeft,
			  std::vector< double >, std::vector< double > > >
	myTable( njoy::utility::copy(xGrid), njoy::utility::copy(yGrid) );

      THEN("the table can be evaluated"){
	REQUIRE( 3.0 == myTable( 1.0 ) );
	REQUIRE( 7.0 == myTable( 3.0 ) );
	
	REQUIRE( 6.5 == myTable( 2.5 ) );
	REQUIRE( 6.5 == myTable( 2.5, myTable.search() ) );
      }

      THEN("the table handles discontinuities"){
	REQUIRE( 5.0 == myTable( 2.0, myTable.search() ) );
      }
    }

    THEN("interpolation tables can cache query results"){
      Table< table::Type< LinearLinear,
			  table::search::Binary,
			  table::discontinuity::TakeLeft,
			  std::vector< double >, std::vector< double > > >
	myTable( njoy::utility::copy(xGrid), njoy::utility::copy(yGrid) );

      auto search = myTable.cachedSearch();
      
      THEN("the table can be evaluated"){
	REQUIRE( 3.0 == myTable( 1.0, search ) );
	REQUIRE( 3.0 == myTable( 1.0, search ) );
	REQUIRE( 7.0 == myTable( 3.0, search ) );
	REQUIRE( 7.0 == myTable( 3.0, search ) );
	
	REQUIRE( 6.5 == myTable( 2.5, search ) );
	REQUIRE( 6.5 == myTable( 2.5, search ) );
      }

      THEN("the table handles discontinuities"){
	REQUIRE( 5.0 == myTable( 2.0, search ) );
	REQUIRE( 5.0 == myTable( 2.0, search ) );
      }
    }

    THEN("interpolation tables can be constructed which transforms each component"){
      auto x = xGrid | ranges::view::transform( applyUnit<Meter> );
      auto y = yGrid | ranges::view::transform( applyUnit<Second> );

      Table< table::Type< LinearLinear,
			  table::search::Binary,
			  table::discontinuity::TakeLeft, 
			  decltype(x), decltype(y) > >
      	myTable( njoy::utility::copy(x), njoy::utility::copy(y) );

      THEN("the table can be evaluated"){
	REQUIRE( 6.5 * seconds == myTable( 2.5 * meter ) );
	REQUIRE( 6.5 * seconds == myTable( 2.5 * meter, myTable.search() ) );
      }

      THEN("the table can provide x-values"){
      	REQUIRE( ranges::equal( myTable.x(), x ) );
      }

      THEN("the table can provide y-values"){
       	REQUIRE( ranges::equal( myTable.y(), y ) );
      }

      THEN("the table can provide interval limits"){
	REQUIRE( myTable.tableMin() == x.front() );
	REQUIRE( myTable.tableMax() == x.back() );
      }
      
      THEN("the domain left limit is negative infinity"){
	REQUIRE( myTable.domainMin() == -infinity< Quantity<Meter> >() );
      }

      THEN("the domain right limit is infinity"){
	REQUIRE( myTable.domainMax() == infinity< Quantity< Meter > >() );
      }
    }
  }
}

SCENARIO("Exceptional behavior"){
  auto construct = []( auto&& x, auto&& y ){
    return Table< table::Type< LinearLinear,
                               table::search::Binary,
                               table::discontinuity::TakeLeft,
                               std::vector< double >, std::vector< double > > >
      ( std::move(x), std::move(y) );
  };

  GIVEN("zero length x-grid"){
    std::vector<double> xgrid{};
    std::vector<double> ygrid{};
    
    THEN("construction will throw"){
      REQUIRE_THROWS( construct( njoy::utility::copy(xgrid),
				 njoy::utility::copy(ygrid) ) );
    }
  }

  GIVEN("mismatched grid lengths"){
    std::vector<double> xgrid{ 1.0, 2.0, 3.0 };
    std::vector<double> ygrid{ 1.0, 2.0, 3.0, 4.0 };
    
    THEN("construction will throw"){
      REQUIRE_THROWS( construct( njoy::utility::copy(xgrid),
				 njoy::utility::copy(ygrid) ) );
    }
  }

  GIVEN("unsorted x-grid"){
    std::vector<double> xgrid{ 1.0, 3.0, 2.0 };
    std::vector<double> ygrid{ 1.0, 2.0, 3.0 };
    
    THEN("construction will throw"){
      REQUIRE_THROWS( construct( njoy::utility::copy(xgrid),
				 njoy::utility::copy(ygrid) ) );
    }
  }
}

SCENARIO("copy/move ctor regression test"){
  std::vector<double> xgrid{ 1.0, 2.0, 3.0 };
  std::vector<double> ygrid{ 1.0, 2.0, 3.0 };
  
  auto reference =
    table::make< LinearLinear,
                 table::search::Hashed< Binary > >( std::move(xgrid),
                                                    std::move(ygrid) );
  auto copy = reference;
  REQUIRE( reference(2.5) == copy(2.5) );

  auto move = std::move(copy);
  REQUIRE( reference(2.5) == move(2.5) );
}
