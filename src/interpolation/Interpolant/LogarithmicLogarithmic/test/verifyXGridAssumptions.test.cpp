#include "catch.hpp"
#include "interpolation.hpp"

using namespace njoy::interpolation;

SCENARIO("The LogarithmicLogarithmic interpolant rejects grids what cross the y-axis",
         "[interpolant], [LogarithmicLogarithmic]"){
  GIVEN("an x-grid of positive values"){
    std::vector< double > xGrid{ 1.0, 2.0, 3.0 };
    THEN("grid verification will succeed"){
      REQUIRE_NOTHROW( LogarithmicLogarithmic::verifyXGridAssumptions( xGrid ) );
    }
    
    WHEN("a zero value is inserted"){
      THEN("grid verification will fail"){
        auto xBegin = xGrid.begin();
        xGrid.insert( xBegin, 0.0 );
        REQUIRE_THROWS( LogarithmicLogarithmic::verifyXGridAssumptions( xGrid ) );
        xBegin = xGrid.begin();
        xGrid.erase( xBegin );
        REQUIRE_NOTHROW( LogarithmicLogarithmic::verifyXGridAssumptions( xGrid ) );
      }
    }
    
    WHEN("a negative value is inserted"){
      THEN("grid verification will fail"){
        auto xBegin = xGrid.begin();
        xGrid.insert( xBegin, -1.0 );
        REQUIRE_THROWS( LogarithmicLogarithmic::verifyXGridAssumptions( xGrid ) );
      }
    }
  }
  
  GIVEN("an x-grid of negative values"){
    std::vector< double > xGrid{ -3.0, -2.0, -1.0 };
    THEN("grid verification will succeed"){
      REQUIRE_NOTHROW( LogarithmicLogarithmic::verifyXGridAssumptions( xGrid ) );
    }
    
    WHEN("a zero value is inserted"){
      THEN("grid verification will fail"){
        xGrid.push_back( 0.0 );
        REQUIRE_THROWS( LogarithmicLogarithmic::verifyXGridAssumptions( xGrid ) );
        xGrid.pop_back();
        REQUIRE_NOTHROW( LogarithmicLogarithmic::verifyXGridAssumptions( xGrid ) );
      }
    }
    
    WHEN("a negative value is inserted"){
      THEN("grid verification will fail"){
        xGrid.push_back( 1.0 );
        REQUIRE_THROWS( LogarithmicLogarithmic::verifyXGridAssumptions( xGrid ) );
      }
    }
  }
}
