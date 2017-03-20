template<>
struct Search< FromRight > {
  template< typename... Args >
  Search( Args&&... ){}
  
  template< typename Range, typename Value >
  static std::decay_t<decltype( std::declval<Range>().begin() )>
  apply( Range&& range, Value&& value){
    /* the ranges library fails us */
    return std::find_if( std::make_reverse_iterator( range.end() ),
			 std::make_reverse_iterator( range.begin() ),
			 [&]( auto&& arg ){ return arg < value; } ).base();
  }

  template< typename Range, typename Value, typename Less >
  static decltype( std::declval<Range>().begin() )
  apply( Range&& range, Value&& value, Less&& less ){
    return std::find_if( std::make_reverse_iterator( range.end() ),
			 std::make_reverse_iterator( range.begin() ),
			 [&]( auto&& arg ){ return less(arg, value); } ).base();
  }
};
