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
    if ( (*left < value) and (value <= *right) ){
      return this->right;
    }

    auto subrange = ( value > *right ) ?
      ranges::make_subrange( right, range.end() ) :
      ranges::make_subrange( range.begin(), right );

    auto iter = core.apply( subrange,
			    std::forward<Value>(value),
			    std::forward<Args>(args)... );
    this->right = ( iter == range.begin() ) ? ranges::cpp20::next(iter) : iter;
    this->left = ranges::cpp20::prev( this->right );
    return iter;
  }

};
