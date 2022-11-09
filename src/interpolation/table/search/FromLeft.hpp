template<>
struct Search< FromLeft > {
  template< typename... Args >
  Search( Args&&... ){}

  template< typename Range, typename Value >
  static auto apply( Range&& range, Value&& value ){
    return ranges::cpp20::find_if( std::forward<Range>(range),
			    [&]( auto&& arg ){ return value <= arg ; } );
  }

  template< typename Range, typename Value, typename Less >
  static auto apply( Range&& range, Value&& value, Less&& less ){
    return ranges::cpp20::find_if( std::forward<Range>(range),
			    [&]( auto&& arg ){ return not less(arg, value); } );
  }
};
