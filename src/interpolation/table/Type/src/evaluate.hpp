template< typename Table, typename Algorithm >
Ydata evaluate( const Xdata& x, Algorithm&& searchAlgorithm ) const {
  const auto xIterator = searchAlgorithm.apply( this->x(), x );
  const auto position  = ranges::distance( this->x().begin(), xIterator );
  const auto yIterator = ranges::next( this->y().begin(), position );

  if ( x != *xIterator ){
    return this->interpolant().apply( x, *ranges::prev( xIterator ), *xIterator,
				         *ranges::prev( yIterator ), *yIterator );
  }

  const bool isLast = ( ranges::next(xIterator) == this->x().end() );
  if ( isLast ){ return *yIterator; }
  
  return ( *xIterator != *ranges::next( xIterator ) ) ? *yIterator :
    this->discontinuity().apply( *yIterator, *ranges::next( yIterator ) );
}
