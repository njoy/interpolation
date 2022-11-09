template< typename Table, typename Algorithm >
Ydata evaluate( const Xdata& x, Algorithm&& searchAlgorithm ) const {
  const auto xIterator = searchAlgorithm.apply( this->x(), x );
  const auto position  = ranges::cpp20::distance( this->x().begin(), xIterator );
  const auto yIterator = ranges::cpp20::next( this->y().begin(), position );

  if ( x != *xIterator ){
    return this->interpolant().apply( x, *ranges::cpp20::prev( xIterator ), *xIterator,
				         *ranges::cpp20::prev( yIterator ), *yIterator );
  }

  const bool isLast = ( ranges::cpp20::next(xIterator) == this->x().end() );
  if ( isLast ){ return *yIterator; }

  return ( *xIterator != *ranges::cpp20::next( xIterator ) ) ? *yIterator :
    this->discontinuity().apply( *yIterator, *ranges::cpp20::next( yIterator ) );
}
