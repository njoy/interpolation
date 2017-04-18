template< typename SearchType, typename... SearchTypes >
class Search {

  template< typename T >
  using Holder =
    std::conditional_t< std::is_lvalue_reference< T >::value,
			std::reference_wrapper<T>, std::decay_t<T> >;

  template< typename T, typename... Args >
  static decltype(auto)
  applyTo( std::reference_wrapper<T>& search, Args&&... args ){
    return search.get().apply( std::forward<Args>(args)... );
  }

  template< typename T, typename... Args >
  static decltype(auto)
  applyTo( const std::reference_wrapper<T>& search, Args&&... args ){
    return search.get().apply( std::forward<Args>(args)... );
  }

  template< typename T, typename... Args >
  static decltype(auto) applyTo( T& search, Args&&... args ){
    return search.apply( std::forward<Args>(args)... );
  }

  template< typename T, typename... Args >
  static decltype(auto) applyTo( const T& search, Args&&... args ){
    return search.apply( std::forward<Args>(args)... );
  }
  
  template< typename T >
  static T deduce( hana::tuple< hana::basic_type< T > > );

  template< typename T, typename U, typename... Ts >
  static std::variant< Holder<T>, Holder<U>, Holder<Ts>... >
  deduce( hana::tuple< hana::basic_type<T>,
	               hana::basic_type<U>,
	               hana::basic_type<Ts>... > );

  template< typename... Ts >
  static decltype
  ( deduce
    ( hana::to_tuple
      ( hana::fold
	( hana::make_tuple
	  ( std::declval< hana::basic_type<Ts> >()... ),
	  hana::make_set(), hana::insert ) ) ) )
  deduce();
  
  template< typename T >
  struct Implementation { using Type = T; };

  template< typename T, typename... Ts >
  struct Implementation< std::variant<T, Ts...> > {
    using Core = std::variant< T, Ts... >;

    class Type {
      Core core;

    public:
      template< typename... Args >
      auto apply( Args&&... args ) const {
	return std::visit( [&]( auto&& self ){
	    return applyTo( self, std::forward<Args>(args)... ); },
	  this->core );
      }

      template< typename Arg >
      Type( Arg&& arg ) : core( std::forward<Arg>( arg ) ){}
      Type( Type& ) = default;
      Type( const Type& ) = default;

      template< typename Arg >
      Type& operator=( const Arg& arg ){ this->core = arg; }
      Type& operator=( Type& ) = default;
      Type& operator=( const Type& ) = default;

      ~Type() = default;    
    };
  };
  
public:
  using Type =
    typename Implementation
    < decltype( deduce< SearchType, SearchTypes... >() ) >::Type;
};
