template< typename IteratorType, typename... IteratorTypes >
class Iterator {

  template< typename T >
  static T deduce( hana::tuple< hana::basic_type< T > > );

  template< typename T, typename U, typename... Ts >
  static std::variant<T, U, Ts...>
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
  struct Implementation {
    using Type = T;
  };

  template< typename T, typename... Ts >
  struct Implementation< std::variant<T, Ts...> > {
    static_assert
    ( all( std::is_same
	   < std::decay_t< decltype( *( std::declval<const T>() ) ) >,
	     std::decay_t< decltype( *( std::declval<const Ts>() ) ) > >::value... ),
      "All iterator parameters types are required to dereference to the same value type" );
    
    using Core = std::variant< T, Ts... >;

    class Type {
      Core core;
      Type& preincrement(){
	std::visit( []( auto& self ){ ++self; }, this->core );
	return *this;
      }
    
      Type postincrement(){
	auto value = *this;
	std::visit( []( auto& self ){ ++self; }, this->core );
	return value;
      }

      Type& predecrement(){
	std::visit( []( auto& self ){ --self; }, this->core );
	return *this;
      }
    
      Type postdecrement(){
	auto value = *this;
	std::visit( []( auto& self ){ --self; }, this->core );
	return value;
      }
      
    public:
      using iterator_category = std::random_access_iterator_tag;
      using value_type = std::remove_reference_t< decltype( *( std::declval<T>() ) ) >;
      using reference = const value_type&;
      using pointer = value_type const *;
      using difference_type = std::ptrdiff_t;
      using result_type =
	std::conditional_t
	< all( std::is_reference
	       < decltype( *( std::declval<const T>() ) ) >::value,
	       std::is_reference
	       < decltype( *( std::declval<const Ts...>() ) ) >::value ),
	  reference, value_type >;      

      template< typename Iterator_t >
      bool operator==( const Iterator_t& other ) const {
	return
	  std::visit
	  ( [&]( const auto& core ){ return equality( core, other ); },
	    this->core );
      }
      
      template< typename Iterator_t >
      bool operator!=( const Iterator_t& other ) const {
	return not (*this == other);
      }
      
      Type& operator++() { return this->preincrement(); }
      Type operator++( int ) { return this->postincrement(); }
      Type& operator--() { return this->predecrement(); }
      Type operator--( int ) { return this->postdecrement(); }

      Type& operator+=( difference_type diff ) {
	std::visit( [&]( const auto& core ){ return  core += diff; },
		    this->core );
	return *this; 
      }

      Type& operator-=( difference_type diff ) {
	std::visit ( [&]( const auto& core ){ return  core -= diff; },
		     this->core );
	return *this;
      }

      Type operator+( difference_type diff ) const {
	auto temp = *this;
	return temp += diff;
      }

      friend Type operator+( difference_type diff, const Type& self ){
	return self + diff;
      }
      
      Type operator-( difference_type diff ) const {
	auto temp = *this;
	return temp -= diff;
      }

      difference_type operator-( const Type& other ) const {
	return std::visit( []( const auto& left, const auto& right ){
	    return difference< difference_type >( left, right );
	  }, this->core, other.core );
      }

      template< typename Iter,
		typename = std::enable_if_t
		< any( Diffable<Iter, T>::value,
		       Diffable<Iter, Ts>::value... ) >,
		typename = utility::void_t
		< decltype( *( std::declval<Iter>() ) ) > >
      difference_type operator-( const Iter& other ) const {
	return std::visit( [&]( const auto& core ){
	    return difference< difference_type >( core, other );
	  }, this->core );
      }

      bool operator<( const Type& other ) const {
	return std::visit( []( const auto& left, const auto& right ){
	    return less( left, right );
	  }, this->core, other.core );
      }

      template< typename Iter,
		typename = std::enable_if_t
		< any( Comparable<Iter, T>::value,
		       Comparable<Iter, Ts>::value... ) >,
		typename = utility::void_t
		< decltype( *( std::declval<Iter>() ) ) > >
      bool operator<( const Iter& other ) const {
	return std::visit( [&]( const auto& core ){
	    return less( core, other );
	  }, this->core );
      }

      template< typename Iter,
		typename = std::enable_if_t
		< any( Comparable<Iter, T>::value,
		       Comparable<Iter, Ts>::value... ) >,
		typename = utility::void_t
		< decltype( *( std::declval<Iter>() ) ) > >
      bool operator<=( const Iter& other ) const {
	return (*this < other) or (*this == other);
      }

      template< typename Iter,
		typename = std::enable_if_t
		< any( Comparable<Iter, T>::value,
		       Comparable<Iter, Ts>::value... ) >,
		typename = utility::void_t
		< decltype( *( std::declval<Iter>() ) ) > >
      bool operator>( const Iter& other ) const {
	return not (*this <= other);
      }
      
      template< typename Iter,
		typename = std::enable_if_t
		< any( Comparable<Iter, T>::value,
		       Comparable<Iter, Ts>::value... ) >,
		typename = utility::void_t
		< decltype( *( std::declval<Iter>() ) ) > >
      bool operator>=( const Iter& other ) const {
	return not ( *this < other );
      }

      result_type operator*() const {
	return std::visit(
	  []( auto&& iterator ) -> result_type { return *iterator; },
	  this->core );
      }

      result_type operator->() const {
	return std::visit(
	  []( auto&& iterator ) -> result_type { return iterator.operator->(); },
	  this->core );
      }
      
      result_type operator[]( difference_type diff ) {
	return std::visit(
	  [&]( const auto& core ) -> result_type { return core[diff]; },
	  this->core );
      }

      template< typename... Args >
      Type( Args&&... args ) : core( std::forward< Args >( args )... ){}
      Type( Type& ) = default;
      Type( Type&& ) = default;
      Type( const Type& ) = default;

      template< typename... Args >
      Type& operator=( const Arg& arg ){ this->core = arg; }
      Type& operator=( Type& ) = default;
      Type& operator=( Type&& ) = default;
      Type& operator=( const Type& ) = default;

      ~Type() = default;    
    };
  };
  
public:
  using Type =
    typename Implementation
    < decltype( deduce< IteratorType, IteratorTypes... >() ) >::Type;
};
