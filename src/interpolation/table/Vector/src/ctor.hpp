Vector( std::vector< TableType >&& data ) : core( std::move(data) ){
  const auto left =
    ranges::cpp20::adjacent_find( this->core, []( const auto& left, const auto& right ){
	return left.x().back() != right.x().front(); } );
  if ( left != this->core.end() ){
    const auto right = ranges::cpp20::next(left);
    if ( left->x().back() < right->x().front() ){
      Log::error( "Encountered gap between component interpolation tables" );
    } else {
      Log::error( "Encountered overlap between component interpolation tables" );
    }
    const auto position = ranges::cpp20::distance( this->core.begin(), left );
    Log::info( "Right edge of x-grid of table[{}]: {}", position, left->x().back() );
    Log::info( "Left edge of x-grid of table[{}]: {}", position + 1, right->x().front() );
    throw std::runtime_error("Encountered non-adjoining tables");
  }
}
