struct Histogram : public Interpolant {
  template< typename Xarg, typename X, typename Y >
  static auto apply( Xarg&&, X&&, X&&, Y&& yLeft, Y&& ){ return yLeft; }

  template< typename Yarg, typename X, typename Y >
  static auto invert( Yarg&&, X&& xLeft, X&&, Y&&, Y&& ){ return xLeft; }

  template< typename Range >
  static void verifyXGridAssumptions( Range&& range ){
    auto it = ranges::adjacent_find( range );
    if ( it != range.end() ){
      Log::error( "Encountered coincident values in x-value grid of histogram" );
      Log::info( "Each entry in x-grid must be unique when applying Histogram interpolation" );
      if ( it == ranges::prev(range.end()) ){ it = ranges::prev(range.end()); }
      const auto position = std::distance( range.begin(), it );
      Log::info( "X-value[ {} ]: {}", position, *it ); 
      Log::info( "X-value[ {} ]: {}", position + 1, *(++it) );
      throw std::logic_error( "Encountered coincident values in x-value grid of histogram" );
    }
  }
};
