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
};

#include "interpolation/table/Vector/Table.hpp"
