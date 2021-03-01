// closing the hosting namespace "table"
}

template< typename... Args, typename... Decorators >
class Table< table::Vector< Args... >, Decorators... > :
  public table::Implementation< table::Vector< Args... >, Decorators... >::Type {

  using Parent =
    typename table::Implementation< table::Vector< Args... >, Decorators... >::Type;

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
  
  template< typename Arg, typename... CallArgs >
  auto operator()( Arg&& x, CallArgs&&... args ) const {
    return Parent:: template evaluate<Parent>( std::forward<Arg>(x),
					       std::forward<CallArgs>(args)... );
  }

  template< typename Xdata, typename... CallArgs >
  auto integrate(const Xdata& xLow, const Xdata& xHi, CallArgs&&... args) const {
    return Parent:: template integrate<Parent>( xLow, xHi,
                              std::forward<CallArgs>(args)...);
  }

};

namespace table {
// reopening the hosting namespace "table"
