// closing the hosting namespace "table"
}

template< typename... Args, typename... Decorators >
class Table< table::Type< Args... >, Decorators... > :
  public table::Implementation< table::Type< Args... >, Decorators... >::Type {

  using Parent =
    typename table::Implementation< table::Type< Args... >, Decorators... >::Type;

public:
  using Ydata = typename Parent::Ydata;
  using Xdata = typename Parent::Xdata;

  static constexpr table::Tag tag(){ return {}; }

  template< typename... CtorArgs >
  Table( CtorArgs&&... args ) :
      Parent( std::forward< CtorArgs >( args )... ){}

  Table( Table& ) = default;
  Table( Table&& ) = default;
  Table( const Table& ) = default;
  
  Table& operator=( Table& ) = default;
  Table& operator=( Table&& ) = default;
  Table& operator=( const Table& ) = default;

  ~Table() = default;

  template< typename... SearchArgs >
  friend struct Search;
  
  template< typename Xdata >
  Ydata operator()( const Xdata& x ) const {
    return Parent:: template evaluate<Parent>( x, Parent::search() );
  }

  template< typename Xdata, typename... CallArgs >
  Ydata operator()( const Xdata& x, CallArgs&&... args ) const {
    return Parent:: template evaluate<Parent>( x,
					       std::forward<CallArgs>(args)... );
  }

  template< typename Xdata >
  auto integrate(const Xdata& xLow, const Xdata& xHi) const {
    return Parent:: template do_integrate<Parent>( xLow, xHi, Parent::search() );
  }
  
};

namespace table {
// reopening the hosting namespace "table"
