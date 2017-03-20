#include <iostream>
#include <vector>

#include "catch.hpp"
#include "interpolation.hpp"

using namespace njoy::interpolation;

TEST_CASE("Example 8"){
  std::vector< double > xGrid = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0 };
  std::vector< double > yGrid = { 3.0, 5.0, 7.0, 6.0, 2.0, 9.0, 8.0, 5.0, 1.0,  0.0 };

  using Partition =
    decltype( xGrid | ranges::view::drop_exactly(0) | ranges::view::take_exactly(0) );

  using Law1 =
    decltype( table::make< Histogram >( std::declval<Partition>(),
                                        std::declval<Partition>() ) );

  using Law2 =
    decltype( table::make< LinearLinear >( std::declval<Partition>(),
                                           std::declval<Partition>() ) );

  using Law3 =
    decltype( table::make< LinearLogarithmic >( std::declval<Partition>(),
                                                std::declval<Partition>() ) );

  using Law4 =
    decltype( table::make< LogarithmicLinear >( std::declval<Partition>(),
                                                std::declval<Partition>() ) );

  using Law5 =
    decltype( table::make< LogarithmicLogarithmic >( std::declval<Partition>(),
                                                     std::declval<Partition>() ) );

  using ENDFvariant = Table< table::Variant< Law1, Law2, Law3, Law4, Law5 > >;

  using Tab1 = Table< table::Vector<ENDFvariant> >;

  auto partition = []( auto&& range, int drop, int take ){
    return range
           | ranges::view::drop_exactly(drop)
           | ranges::view::take_exactly(take);
  };

  std::vector< ENDFvariant > core;
  core.push_back( Law2( partition( xGrid, 0, 3 ), partition( yGrid, 0, 3 ) ) );
  core.push_back( Law5( partition( xGrid, 2, 4 ), partition( yGrid, 2, 4 ) ) );
  core.push_back( Law3( partition( xGrid, 5, 5 ), partition( yGrid, 5, 5 ) ) );
  Tab1 myTable( std::move(core) );

  REQUIRE( myTable( 2.5 ) == 6.0 );
}
