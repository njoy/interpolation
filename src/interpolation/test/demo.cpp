#include <iostream>
#include <vector>
#include <tuple>

#define CATCH_CONFIG_MAIN

#include "dimwits.hpp"
#include "boost/hana.hpp"
#include "boost/hana/ext/std/tuple.hpp"
#include "boost/hana/ext/std/pair.hpp"

#include "range/v3/range/conversion.hpp"
#include "range/v3/view/take_exactly.hpp"
#include "range/v3/view/zip.hpp"

#include "header-utilities/copy.hpp"

template< typename T >
struct echo;

template< typename... Ts >
std::ostream& operator<<( std::ostream& os, const std::pair<Ts...>& args ){
  os << "{ ";
  boost::hana::for_each( args, [&]( auto&& arg ){ os << arg << ' '; } );
  os << '}';
  return os;
}

template< typename... Ts >
std::ostream& operator<<( std::ostream& os, const std::tuple<Ts...>& args ){
  os << "{ ";
  boost::hana::for_each( args, [&]( auto&& arg ){ os << arg << ' '; } );
  os << '}';
  return os;
}

template< typename T >
std::ostream& operator<<( std::ostream& os, const std::vector<T>& args ){
  os << "[ ";
  for( const auto& arg : args ){ os << arg << ' '; }
  os << ']';
  return os;
}

#include "interpolation.hpp"
#include "catch.hpp"

using namespace njoy;
using namespace dimwits;

SCENARIO("Lets zip some vectors"){
  std::vector< double > XSS
  { 1.,  2.,  3.,  4.,  5.,  6.,  7.,  8.,
    3.,  7., 11., 15., 19., 23., 27., 31.,
    2.,  4.,  6.,  8., 10., 12., 14., 16.,
    1.,  3.,  5.,  7.,  9., 11., 13., 15. };

  const int size = 8;

  auto energyView = XSS
    | ranges::views::take_exactly( size );

  std::cout << energyView << std::endl;

  auto totalView = XSS
    | ranges::views::drop_exactly( size )
    | ranges::views::take_exactly( size );

  std::cout << totalView << std::endl;

  auto absorptionView = XSS
    | ranges::views::drop_exactly( size * 2 )
    | ranges::views::take_exactly( size );

  std::cout << absorptionView << std::endl;

  auto scatterView = XSS
    | ranges::views::drop_exactly( size * 3 )
    | ranges::views::take_exactly( size );

  std::cout << scatterView << std::endl;

  auto zippedVector =
    ranges::to< std::vector< std::tuple< double, double, double, double > > >(
      ranges::views::zip( energyView, scatterView, absorptionView, totalView ) );

  std::cout << zippedVector << std::endl;

  auto energy = zippedVector
    | ranges::cpp20::views::transform( []( const auto& arg ){
	return std::get<0>(arg) * mega(electronVolts); } );

  std::cout << energy << std::endl;

  auto scattering = zippedVector
    | ranges::cpp20::views::transform( []( const auto& arg ){
	return std::get<1>(arg) * barn; } );

  std::cout << scattering << std::endl;

  auto absorption = zippedVector
    | ranges::cpp20::views::transform( []( const auto& arg ){
	return std::get<2>(arg) * barn; } );

  std::cout << absorption << std::endl;

  auto total = zippedVector
    | ranges::cpp20::views::transform( []( const auto& arg ){
	return std::get<3>(arg) * barn; } );

  std::cout << total << std::endl;

  auto totalXS =
    interpolation::table::make
    < interpolation::LinearLinear >( utility::copy(energy),
				     utility::copy(total) );

  REQUIRE( 13.0 * barn == totalXS( 3.5E3 * kilo(electronVolts) ) );

  auto scatteringXS =
    interpolation::table::make
    < interpolation::LinearLinear >( utility::copy(energy),
				     utility::copy(scattering) );

  REQUIRE( 6.0 * barn == scatteringXS( 3.5 * mega(electronVolts) ) );

  auto absorptionXS =
    interpolation::table::make
    < interpolation::LinearLinear >( utility::copy(energy),
				     utility::copy(absorption) );

  REQUIRE( 7.0 * barn == absorptionXS( 3.5 * mega(electronVolts) ) );

}
