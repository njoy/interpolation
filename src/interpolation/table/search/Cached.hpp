template< typename Iterator, typename Core >
struct Search< Cached, Iterator, Core > {
protected:
  Iterator left, right;
  Core core;

public:
  Search( Iterator left, Iterator right, Core search ) :
  left( left ), right( right ), core( search ){}
  
  template< typename Range, typename Value, typename... Args >
  decltype(auto) apply( Range&& range, Value&& value, Args&&... args ){
    if ( (*left < value) and (value < *right) ){
      return this->right;
    }
    this->right = core.apply( std::forward<Range>(range),
			      std::forward<Value>(value),
			      std::forward<Args>(args)... );
    this->left = ranges::prev( this->right );
    return this->right;
  }

};