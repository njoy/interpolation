template< typename InterpolationAlgorithm,
	  typename SearchAlgorithm,
	  typename DiscontinuityAlgorithm,
	  typename Xrange, typename Yrange >
class Type {
  Xrange xRange;
  Yrange yRange;
  SearchAlgorithm searchAlgorithm;

public:
  using Xdata = std::decay_t< decltype( *( std::declval<Xrange>().begin() ) ) >;
  using Ydata = std::decay_t< decltype( *( std::declval<Yrange>().begin() ) ) >;
  
  #include "interpolation/table/Type/src/ctor.hpp"

  static constexpr auto interpolant(){ return InterpolationAlgorithm(); }
  static constexpr auto discontinuity(){ return DiscontinuityAlgorithm(); }

  static constexpr bool specifiesLeftInterval(){ return false; }
  static constexpr bool specifiesRightInterval(){ return false; }
  static constexpr bool specifiesDomainMin(){ return false; }
  static constexpr bool specifiesDomainMax(){ return false; }
  
  const Xrange& x() const { return this->xRange; }
  const Yrange& y() const { return this->yRange; }
  const SearchAlgorithm& search() const { return this->searchAlgorithm; }
  
  auto cachedSearch() const {
    using Iterator =
      std::decay_t< decltype( std::declval<const Xrange>().begin() ) >;
    return Search< Cached, Iterator, const SearchAlgorithm& >( this->xRange.begin(), this->xRange.begin(), this->search() );
  } 

  Xdata tableMin() const { return this->xRange.front(); }
  Xdata tableMax() const { return this->xRange.back(); }
  
  static constexpr Xdata domainMin(){ return -infinity<Xdata>(); }
  static constexpr Xdata domainMax(){ return infinity<Xdata>(); }

  #include "interpolation/table/Type/src/evaluate.hpp"
};

template< typename InterpolationAlgorithm,
	  typename SearchTag,
	  typename DiscontinuityAlgorithm,
	  typename Xrange, typename Yrange >
class Type< InterpolationAlgorithm, Search< Hashed< SearchTag > >,
	    DiscontinuityAlgorithm, Xrange, Yrange > :
  public Type< InterpolationAlgorithm,
	       typename Search< Hashed< SearchTag > >::template Type< Xrange >,
	       DiscontinuityAlgorithm, Xrange, Yrange >{
public:
  Type( Xrange&& xRange, Yrange&& yRange ) :
    Type< InterpolationAlgorithm,
	  typename Search< Hashed< SearchTag > >:: template Type< Xrange >,
	  DiscontinuityAlgorithm, Xrange, Yrange >
    ( std::forward<Xrange>( xRange ), std::forward<Yrange>( yRange ) ){}
};

#include "interpolation/table/Type/Table.hpp"  
#include "interpolation/table/Type/Make.hpp"  
