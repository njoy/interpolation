inline constexpr bool any(){ return false; }
  
template< typename T, typename... Ts >
constexpr bool any( T&& t, Ts&&... ts ){
  return t or any( std::forward<Ts>(ts)... );     
}

inline constexpr bool all(){ return true; }
  
template< typename T, typename... Ts >
constexpr bool all( T&& t, Ts&&... ts ){
  return t and all( std::forward<Ts>(ts)... );     
}

template< typename Left, typename Right, typename = void >
struct Equitable{
  static constexpr bool value = false;
};

template< typename Left, typename Right >
struct Equitable
< Left, Right, utility::void_t
  < decltype( std::declval< std::decay_t<Left> >()
	      == std::declval< std::decay_t<Right> >() ) > >{
  static constexpr bool value = true;
};

template< typename Left, typename Right,
	  typename = std::enable_if_t < Equitable<Left, Right>::value > >
bool equality( Left&& left, Right&& right ){ return left == right; }

template< typename Left, typename Right, typename... Args,
	  typename = std::enable_if_t < Equitable<Right, Left>::value > >
bool equality( Left&& left, Right&& right, Args... ){ return right == left; }

inline bool equality( ... ){ return false; }

template< typename Left, typename Right, typename = void >
struct Comparable {
  static constexpr bool value = false;
};

template< typename Left, typename Right >
struct Comparable
< Left, Right, utility::void_t
  < decltype( std::declval< std::decay_t<Left> >()
	      < std::declval< std::decay_t<Right> >() ) > >{
  static constexpr bool value = true;
};

template< typename Left, typename Right,
	  typename = std::enable_if_t< Comparable< Left, Right >::value > >
bool less( Left&& left, Right&& right ){
  return left < right;
}

template< typename Left, typename Right, typename... Args,
	  typename = std::enable_if_t< Comparable< Right, Left >::value > >
bool less( Left&& left, Right&& right, Args... ){
  return not ( right < left or equality( std::forward<Left>(left),
					 std::forward<Right>(right) ) );
}

inline bool less( ... ){ throw std::logic_error("Inputs are not comparable"); }

template< typename Left, typename Right, typename = void >
struct Diffable{
  static constexpr bool value = false;
};

template< typename Left, typename Right >
struct Diffable
< Left, Right, utility::void_t
  < decltype( std::declval< std::decay_t<Left> >()
	      - std::declval< std::decay_t<Right> >() ) > >{
  static constexpr bool value = true;
};

template< typename difference_type, typename Left, typename Right,
	  typename = std::enable_if_t< Diffable< Left, Right >::value > >
difference_type difference( Left&& left, Right&& right ){
  return left - right;
}

template< typename difference_type >
difference_type difference( ... ){
  throw std::logic_error("Inputs are not diffable");
}
