#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "interpolation.hpp"

#include "dimwits.hpp"

using namespace njoy::interpolation;
using namespace dimwits;

std::vector<double> list{ 0.0, 1.0, 2.0, 2.0, 2.0, 3.0 };

SCENARIO("Binary"){
  SECTION("double precision floats"){
    auto iterator = table::search::Binary::apply( list, 2.0 );
    REQUIRE( ranges::cpp20::distance( list.begin(), iterator ) == 2 );
  }
  SECTION("double precision floats with units"){
    auto meterList =
      list | ranges::cpp20::views::transform( []( auto&& arg ){ return arg * meters; } );
    auto iterator = table::search::Binary::apply( meterList, 2.0 * meters );
    REQUIRE( ranges::cpp20::distance( meterList.begin(), iterator ) == 2 );
  }
}

SCENARIO("FromLeft"){
  SECTION("double precision floats"){
    auto iterator = table::search::FromLeft::apply( list, 2.0 );
    REQUIRE( ranges::cpp20::distance( list.begin(), iterator ) == 2 );
  }
  SECTION("double precision floats with units"){
    auto meterList =
      list | ranges::cpp20::views::transform( []( auto&& arg ){ return arg * meters; } );
    auto iterator = table::search::FromLeft::apply( meterList, 2.0 * meters );
    REQUIRE( ranges::cpp20::distance( meterList.begin(), iterator ) == 2 );
  }
}

SCENARIO("FromRight"){
  SECTION("double precision floats"){
    auto iterator = table::search::FromRight::apply( list, 2.0 );
    REQUIRE( ranges::cpp20::distance( list.begin(), iterator ) == 2 );
  }
  SECTION("double precision floats with units"){
    auto meterList =
      list | ranges::cpp20::views::transform( []( auto&& arg ){ return arg * meters; } );
    auto iterator = table::search::FromRight::apply( meterList, 2.0 * meters );
    REQUIRE( ranges::cpp20::distance( meterList.begin(), iterator ) == 2 );
  }
}

SCENARIO("Hashed"){
  std::vector< double > list;
  list.reserve( 100 ); list.push_back( 1.0 );
  int count = 100;

  while ( --count ){ list.push_back( list.back() * 1.04712854805 );  }

  table::Search< Hashed<FromRight> >::Type< std::vector<double> > search( list );

  auto position = search.apply( list, 25.0 ) - list.begin();
  REQUIRE( position == 70 );
}
