#include "catch.hpp"
#include "interpolation.hpp"

using namespace njoy::interpolation;

SCENARIO("The LogarithmicLogarithmic interpolant rejects grids what cross the x-axis",
         "[interpolant], [LogarithmicLogarithmic]"){
  GIVEN("an y-grid of positive values"){
    std::vector< double > yGrid{ 1.0, 2.0, 3.0 };
    THEN("grid verification will succeed"){
      REQUIRE_NOTHROW( LogarithmicLogarithmic::verifyYGridAssumptions( yGrid ) );
    }
    WHEN("a zero value is inserted"){
      THEN("grid verification will fail"){
        auto yBegin = yGrid.begin();
        yGrid.insert( yBegin, 0.0 );
        REQUIRE_THROWS( LogarithmicLogarithmic::verifyYGridAssumptions( yGrid ) );
        yBegin = yGrid.begin();
        yGrid.erase( yBegin );
        REQUIRE_NOTHROW( LogarithmicLogarithmic::verifyYGridAssumptions( yGrid ) );
      }
    }
    WHEN("a negative value is inserted"){
      THEN("grid verification will fail"){
        auto yBegin = yGrid.begin();
        yGrid.insert( yBegin, -1.0 );
        REQUIRE_THROWS( LogarithmicLogarithmic::verifyYGridAssumptions( yGrid ) );
      }
    }
  }
  GIVEN("an y-grid of negative values"){
    std::vector< double > yGrid{ -3.0, -2.0, -1.0 };
    THEN("grid verification will succeed"){
      REQUIRE_NOTHROW( LogarithmicLogarithmic::verifyYGridAssumptions( yGrid ) );
    }
    WHEN("a zero value is inserted"){
      THEN("grid verification will fail"){
        yGrid.push_back( 0.0 );
        REQUIRE_THROWS( LogarithmicLogarithmic::verifyYGridAssumptions( yGrid ) );
        yGrid.pop_back();
        REQUIRE_NOTHROW( LogarithmicLogarithmic::verifyYGridAssumptions( yGrid ) );
      }
    }
    WHEN("a negative value is inserted"){
      THEN("grid verification will fail"){
        yGrid.push_back( 1.0 );
        REQUIRE_THROWS( LogarithmicLogarithmic::verifyYGridAssumptions( yGrid ) );
      }
    }
  }
}
