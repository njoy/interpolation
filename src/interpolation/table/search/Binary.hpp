template<>
struct Search< Binary > {
  template< typename... Args >
  Search( Args&&... ){}
  
  template< typename Range, typename Value >
  static auto apply( Range&& range, Value&& value){
    return ranges::lower_bound( std::forward<Range>(range),
				std::forward<Value>(value) );
  }

  template< typename Range, typename Value, typename Less >
  static auto apply( Range&& range, Value&& value, Less&& less ){
    return ranges::lower_bound( std::forward<Range>(range),
				std::forward<Value>(value),
				std::forward<Less>(less) );
  }
};
