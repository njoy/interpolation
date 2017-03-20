// closing the hosting namespace "table"
}

template< typename... Args, typename... Decorators >
class Table< table::Variant< Args... >, Decorators... > :
  public table::Implementation< table::Variant< Args... >, Decorators... >::Type {

  using Parent =
    typename table::Implementation< table::Variant< Args... >, Decorators... >::Type;

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

  template< typename Arg, typename... CallArgs >
  auto operator()( Arg&& x, CallArgs&&... args ) const {
    return Parent:: template evaluate<Parent>( std::forward<Arg>(x),
					       std::forward<CallArgs>(args)... );
  }
};

namespace table {
// reopening the hosting namespace "table"
