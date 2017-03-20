namespace transform {

template< typename Transform >
struct X {
  
  template< typename Parent >
  class Type : public Parent {
  protected:
    template< typename Table, typename Arg, typename... Args >
    auto evaluate( Arg&& x, Args&&... args ) const {
      return Parent::template evaluate<Parent>
	( Transform::invert( std::forward<Arg>(x) ),
	  std::forward<Args>(args)... );
    }
    
  public:
    using Xdata =
      decltype( Transform::apply( std::declval< typename Parent::Xdata >() ) );
    
    auto x() const {
      return Parent::x() | ranges::view::transform(
	[](auto&& arg ){ return Transform::apply( arg ); } );
    }

    auto tableMin() const { return Transform::apply( Parent::tableMin() ); }
    auto tableMax() const { return Transform::apply( Parent::tableMax() ); }
    auto domainMin() const { return Transform::apply( Parent::domainMin() ); }
    auto domainMax() const { return Transform::apply( Parent::domainMax() ); }
    
    template< typename... Args > 
    Type( Args&&... args ) : 
      Parent( std::forward< Args >( args )... ){}  
  };
  
};

template< typename Transform >
struct Y {

  template< typename Parent >
  class Type : public Parent {
  protected:
    template< typename Table, typename Arg, typename... Args >
    auto evaluate( Arg&& x, Args&&... args ) const {
      return Transform::apply
	( Parent::template evaluate<Parent>( std::forward<Arg>(x),
					     std::forward<Args>(args)... ) );
    }
    
  public:
    using Ydata =
      decltype( Transform::apply( std::declval< typename Parent::Ydata >() ) );
    
    auto y() const {
      return Parent::y() | ranges::view::transform(
	[](auto&& arg ){ return Transform::apply( arg ); } );
    }
    
    template< typename... Args >
    Type( Args&&... args ) :
      Parent( std::forward< Args >( args )... ){}
  };

};

}
