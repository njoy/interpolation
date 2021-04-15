template< typename Tag >
struct Search< Hashed<Tag> > {

  template< typename Range >
  struct Type {
  protected:
    using Iterator = decltype( std::declval<const Range>().begin() );
    using Value = decltype( *( std::declval<const Range>().begin() ) );
    using Element =
      decltype( ranges::make_subrange( std::declval<Iterator>(),
					     std::declval<Iterator>() ) );

    std::vector< Element > table;
    Value normalization;

  static std::vector< Element > populate( const Range& range ){
    std::vector< Element > table;
    auto first = range.front();
    auto zero = 0 * first;
    if( first <= zero ){ throw std::runtime_error(""); }
    auto left = range.begin();
    int reference = 0;
    for ( auto iterator = ranges::cpp20::next(left);
	  iterator != range.end();
	  ++iterator ){
      int trial = std::floor( std::log( *iterator / first ) );
      auto right = ranges::cpp20::next(iterator);
      if ( trial != reference ){
	auto step = trial - reference;
	while ( step-- ){ table.emplace_back( left, right ); }
	left = iterator;
	reference = trial;
      }
    }
    table.emplace_back( left, range.end() );
    return table;
  }

  public:
    Type( const Range& range ) :
      table( populate(range) ), normalization( range.front() ){}

    template< typename Range_t, typename Value >
    auto apply( Range_t&&, Value&& value ) const {
      const auto index = std::floor( std::log( value / this->normalization ) );
      return Search<Tag>::apply( this->table[index], value );
    }
  };

};
