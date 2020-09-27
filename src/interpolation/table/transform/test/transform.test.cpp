#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "interpolation.hpp"

#include <numeric>
#include "range/v3/algorithm/for_each.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/view/zip.hpp"

using namespace njoy::interpolation;

struct Zero {
  static constexpr double value = 0.0;
};

struct Ten {
  static constexpr double value = 10.0;
};

template< typename Interp, typename Xrange, typename Yrange = Xrange >
using Base =
  table::Type< Interp,
	       table::search::Binary,
	       table::discontinuity::TakeLeft,
	       Xrange, Yrange >;

struct Exponential {
  template< typename T >
  static auto apply( T&& arg ){ return std::exp(arg); }

  template< typename T >
  static auto invert( T&& arg ){ return std::log(arg); }
};

SCENARIO("An interpolation table can be constructed with transforms"){
  GIVEN("an x- and y-grid"){

    std::vector< double > xGrid{1.0, 2.0, 3.0};
    std::vector< double > yGrid{3.0, 5.0, 7.0};

    WHEN("when subject to exponentation"){
      Table< table::Type< LinearLinear,
			  table::search::Binary,
			  table::discontinuity::TakeLeft,
			  std::vector< double >,
			  std::vector< double > >,
	     table::transform::X< Exponential >,
	     table::transform::Y< Exponential > >
	trial( njoy::utility::copy(xGrid), njoy::utility::copy(yGrid) );

      THEN("post-transforms agree with pre-transforms"){
	auto exponential = []( auto&& arg ){ return std::exp(arg); };
	auto expX = xGrid | ranges::view::transform( exponential );
	auto expY = yGrid | ranges::view::transform( exponential );

	Table< table::Type< LinearLinear,
			    table::search::Binary,
			    table::discontinuity::TakeLeft,
			    decltype(expX), decltype(expY) > >
	  reference( njoy::utility::copy(expX), njoy::utility::copy(expY) );

	ranges::for_each( ranges::view::zip( reference.x(), trial.x() ),
			  []( auto&& pair ){
			    REQUIRE( ( std::get<0>(pair) == std::get<1>(pair) ) );
			  } );

	ranges::for_each( ranges::view::zip( reference.y(), trial.y() ),
			  []( auto&& pair ){
			    REQUIRE( ( std::get<0>(pair) == std::get<1>(pair) ) );
			  } );
      }

      THEN("interpolation reflects the transforms"){
	REQUIRE( std::exp(4.0) == trial( std::exp(1.5), trial.search() ) ) ;
      }

      THEN("lifetime of iterator is not limited to the "){
	auto persistance = []( auto&& arg ){ return arg.begin(); };
	auto maybe = persistance( trial.x() );
	auto difference =
	  std::accumulate( maybe, trial.x().end(), 0.0 )
	  - ranges::accumulate( trial.x(), 0.0 );
	REQUIRE( difference == 0.0 );
      }
    }
  }
}

#include "dimwits.hpp"

using namespace dimwits;

template< typename Unit >
struct ApplyUnit{
  template< typename Magnitude >
  static constexpr auto
  apply( Magnitude&& magnitude ) {
    Quantity< Unit, std::decay_t<Magnitude> > q;
    q.value = magnitude;
    return q;
  }

  template< typename Magnitude >
  static constexpr auto
  invert( const Quantity< Unit, Magnitude >& q ) {
    return q.value;
  }

  template< typename Magnitude >
  constexpr auto
  operator()( Magnitude&& magnitude ) const {
    return apply( std::forward<Magnitude>(magnitude) );
  }
};

template< typename Unit >
constexpr auto applyUnit = ApplyUnit<Unit>{};

SCENARIO("Transforms can change the input and output types"){
  GIVEN("an x- and y-grid"){
    std::vector< double > xGrid{1.0, 2.0, 3.0};
    std::vector< double > yGrid{3.0, 5.0, 7.0};

    WHEN("when subject to type-changing transform"){
      Table< table::Type< LinearLinear,
			  table::search::Binary,
			  table::discontinuity::TakeLeft,
			  std::vector< double >,
			  std::vector< double > >,
	     table::transform::X< ApplyUnit<Meter> >,
	     table::transform::Y< ApplyUnit<Second> > >
	trial( njoy::utility::copy(xGrid), njoy::utility::copy(yGrid) );

      THEN("post-transforms agree with pre-transforms"){
	auto xMeters = xGrid | ranges::view::transform( applyUnit<Meter> );
	auto ySeconds = yGrid | ranges::view::transform( applyUnit<Second> );

	Table< table::Type< LogarithmicLogarithmic,
			    table::search::Binary,
			    table::discontinuity::TakeLeft,
			    decltype(xMeters), decltype(ySeconds) > >
	reference( njoy::utility::copy(xMeters), njoy::utility::copy(ySeconds) );

	ranges::for_each( ranges::view::zip( reference.x(), trial.x() ),
			  []( auto&& pair ){
			    REQUIRE( ( std::get<0>(pair) == std::get<1>(pair) ) );
			  } );

	ranges::for_each( ranges::view::zip( reference.y(), trial.y() ),
			  []( auto&& pair ){
			    REQUIRE( ( std::get<0>(pair) == std::get<1>(pair) ) );
			  } );
      }
    }
  }
}
