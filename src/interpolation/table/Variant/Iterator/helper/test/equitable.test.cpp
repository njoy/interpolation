#include "catch.hpp"
#include "interpolation.hpp"

using namespace njoy::interpolation::table::variant;

namespace {

struct Left{};
struct Right{
  bool operator==( Left ){ return true; }
};

}

SCENARIO("equitable"){
  bool correct = Equitable< double, double >::value;
  REQUIRE( correct );
  correct = Equitable< int, double >::value;
  REQUIRE( correct );
  correct = Equitable< double, int >::value;
  REQUIRE( correct );
  correct = Equitable< int, int >::value;
  REQUIRE( correct );
  correct = not Equitable< Left, Right >::value;
  REQUIRE( correct );
  correct = Equitable< Right, Left >::value;
  REQUIRE( correct );

  Left left;
  Right right;
  REQUIRE( right == left );
  REQUIRE( equality(right, left) );
  REQUIRE( equality(left, right) );
}
