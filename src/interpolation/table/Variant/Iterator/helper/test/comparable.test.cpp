#include "catch.hpp"
#include "interpolation.hpp"

using namespace njoy::interpolation::table::variant;

namespace {

struct Left{};
struct Right{
  bool operator<( Left ){ return true; }
};

}

SCENARIO("comparable"){
  bool correct = Comparable< double, double >::value;
  REQUIRE( correct );
  correct = Comparable< int, double >::value;
  REQUIRE( correct );
  correct = Comparable< double, int >::value;
  REQUIRE( correct );
  correct = Comparable< int, int >::value;
  REQUIRE( correct );
  correct = not Comparable< Left, Right >::value;
  REQUIRE( correct );
  correct = Comparable< Right, Left >::value;
  REQUIRE( correct );

  Left left;
  Right right;
  REQUIRE( right < left );
  REQUIRE( less(right, left) );
  REQUIRE( not less(left, right) );
}
