#include "catch.hpp"
#include "interpolation.hpp"

using namespace njoy::interpolation::table::variant;

namespace {

struct Left{};
struct Right{
  int operator-( Left ){ return 10; }
};

}

SCENARIO("diffable"){
  bool correct = Diffable< double, double >::value;
  REQUIRE( correct );
  correct = Diffable< int, double >::value;
  REQUIRE( correct );
  correct = Diffable< double, int >::value;
  REQUIRE( correct );
  correct = Diffable< int, int >::value;
  REQUIRE( correct );
  correct = not Diffable< Left, Right >::value;
  REQUIRE( correct );
  correct = Diffable< Right, Left >::value;
  REQUIRE( correct );

  Left left;
  Right right;
  REQUIRE( right - left == 10 );
  REQUIRE( difference<int>(right, left) == 10 );
  REQUIRE_THROWS( difference<int>(left, right) );
}
