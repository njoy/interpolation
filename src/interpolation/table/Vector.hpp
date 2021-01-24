template< typename TableType, typename Tag = Adjoining >
struct Vector;

template< typename TableType >
struct Vector< TableType, Adjoining > {

  using ComponentXrange = decltype( std::declval< TableType >().x() );
  using ComponentYrange = decltype( std::declval< TableType >().y() );
  
  template< typename Functor >
  static auto concatenate( const std::vector< TableType >& core,
			   Functor&& functor ) {
    auto strip = [&]( auto&& view ){
      return functor(view) | ranges::view::drop_exactly(1);
    };

    decltype(auto) head = functor( core.front() );
    
    auto tail = core
      | ranges::view::drop_exactly(1)
      | ranges::view::transform( strip )
      | ranges::view::join;
    
    return ranges::view::concat( head, tail );
  }

  static auto getX(){ return []( auto&& view )->ComponentXrange{ return view.x(); }; }
  static auto getY(){ return []( auto&& view )->ComponentYrange{ return view.y(); }; }
  
  std::vector< TableType > core;
   
public:
  using Xdata = typename TableType::Xdata;
  using Ydata = typename TableType::Ydata;

  #include "interpolation/table/Vector/src/ctor.hpp"
  
  static constexpr bool specifiesLeftInterval(){ return false; }
  static constexpr bool specifiesRightInterval(){ return false; }
  static constexpr bool specifiesDomainMin(){ return false; }
  static constexpr bool specifiesDomainMax(){ return false; }

  auto x() const { return concatenate( this->core, getX() ); }
  auto y() const { return concatenate( this->core, getY() ); }

  Xdata tableMin() const { return this->core.front().tableMin(); }
  Xdata tableMax() const { return this->core.back().tableMax(); }
  
  static constexpr Xdata domainMin(){ return -infinity<Xdata>(); }

  static constexpr Xdata domainMax(){ return infinity<Xdata>(); }

  template< typename Table, typename Arg, typename... Args >
  auto evaluate( Arg&& x, Args&&... args ) const {
    auto iterator =
      ranges::lower_bound( this->core, x, ranges::less(),
			   []( auto&& table ){ return table.tableMax(); } );
  
    return (*iterator)( std::forward<Arg>(x), std::forward<Args>(args)... );
  }

  template< typename Table, typename Arg, typename... Args >
  auto integrate( Arg&& xL, Arg&& xH, Args&&... args ) const {
    auto xLow = xL;
    auto xHi = xH;
    const bool reversed = xLow > xHi;

    if ( reversed ) {
      auto xLowTmp = xLow;
      xLow = xHi;
      xHi = xLowTmp;
    }

    // Integration may only be carried out over the function's valid domain
    if (xLow < tableMin()) {
      xLow = tableMin();
    } else if (xLow > tableMax()) {
      xLow = tableMax();
    }

    if (xHi > tableMax()) {
      xHi = tableMax();
    } else if (xHi < tableMin()) {
      xHi = tableMin();
    }

    // Get region which contains xLow
    auto region = ranges::lower_bound(this->core, xLow, ranges::less(),
                  []( auto&& table ){ return table.tableMax(); } );

    if (region->tableMin() > xLow) {
      region = ranges::prev(region);
    }

    auto integral = (xLow - xLow) * region->y().front();
    double xLowLim = xLow;
    double xUppLim = xHi;
    bool integrating = true;
    while (integrating) {
      if (xLowLim < region->tableMin()) {
        xLowLim = region->tableMin();
      }
      
      if (xUppLim > region->tableMax()) {
        xUppLim = region->tableMax();
      }

      integral += region->integrate(xLowLim, xUppLim);

      if (xUppLim == xHi) {
        integrating = false;
      } else {
        xLowLim = xUppLim;
        xUppLim = xHi;
        region = ranges::next(region);
      }
    }

    if (reversed) {
      integral *= -1.;
    }

    return integral;
  }

  auto cachedSearch() const {
    return this->core.front().cachedSearch();
  }
  
};

#include "interpolation/table/Vector/Table.hpp"
