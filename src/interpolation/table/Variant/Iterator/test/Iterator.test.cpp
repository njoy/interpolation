#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "interpolation.hpp"

using namespace njoy::interpolation::table::variant;

namespace hana = boost::hana;

SCENARIO("Degenerate"){
  using viter = std::vector<double>::iterator;
  bool same = std::is_same< Iterator< viter >::Type, viter >::value;
  REQUIRE(same);
  
  same = std::is_same< Iterator< viter, viter >::Type, viter >::value;
  REQUIRE(same);

  using aiter = std::array<double, 10>::iterator;
  
  same = std::is_same< Iterator< double*, aiter >::Type, aiter >::value;
  REQUIRE(same);
}

template< typename T >
struct echo;

SCENARIO("Elaborate"){
  std::vector<int> vi{ 0, 1, 2, 3 };
  auto vt = vi | ranges::view::transform([]( auto&& arg ){ return arg + 1; });
  std::array<int, 4> ai{{ 0, 1, 2, 3 }};

  using viter = std::vector<int>::iterator;
  using vtiter = decltype( vt.begin() );
  using aiter = std::array<int, 10>::iterator;
  
  bool deref = 
    std::is_same< int&, decltype( *( std::declval< viter >() ) ) >::value;

  REQUIRE( deref );
  
  deref = 
    std::is_same
    < const int&,
      decltype( *( std::declval< Iterator< viter, aiter >::Type >() ) ) >::value;

  REQUIRE( deref );
  
  deref = 
    not std::is_same
    < int,
      decltype( *( std::declval< Iterator< viter, aiter >::Type >() ) ) >::value;

  REQUIRE( deref );
  
  deref = 
    std::is_same
    < int,
      decltype( *( std::declval< Iterator< vtiter, aiter >::Type >() ) ) >::value;

  REQUIRE( deref );

  Iterator< viter, aiter >::Type iter = vi.begin();

  bool is_equal = std::equal( vi.begin(), vi.end(), iter );
  REQUIRE( is_equal );
}
