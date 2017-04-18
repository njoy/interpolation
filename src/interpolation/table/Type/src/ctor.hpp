Type( Xrange&& xRange, Yrange&& yRange ) :
  xRange( std::move(xRange) ),
  yRange( std::move(yRange) ),
  searchAlgorithm( this->xRange ){
  try {
    const auto xWidth = ranges::distance( this->xRange );
    if ( xWidth == 0 ){
      Log::error( "Grid of x-value has zero entries" );
      throw std::runtime_error("Grid of x-value has zero entries");
    }
    
    const auto yWidth = ranges::distance( this->yRange );
    if ( xWidth != yWidth ){
      Log::error( "Inconsistent lengths of x-value and y-value grids" );
      Log::info( "Length of x-value grid: {}", xWidth );
      Log::info( "Length of y-value grid: {}", yWidth );
      throw std::runtime_error("Inconsistent lengths of x-value and y-value grids");
    }

    if ( not ranges::is_sorted( this->xRange ) ){
      const auto xIterator = ranges::is_sorted_until( this->xRange );
      const auto position  = ranges::distance( this->xRange.begin(), xIterator );
      Log::error( "Unsorted values encountered in x-value grid" );
      Log::info( "Position of out-of-order value: {}", position );
      // Log::info( "X-value[ {} ] : {}", position - 1, *ranges::prev( xIterator ) );
      // Log::info( "X-value[ {} ] : {}", position, *xIterator );
      throw std::runtime_error("Unsorted values encountered in x-value grid");
    }

    this->interpolant().verifyXGridAssumptions( this->xRange );
    this->interpolant().verifyYGridAssumptions( this->yRange );
    
  } catch ( std::exception& e ) {
    Log::info( "Error while constructing interpolation table" );
    throw e;
  }
}
