Type( Xrange&& xRange, Yrange&& yRange ) :
  xRange( std::move(xRange) ),
  yRange( std::move(yRange) ),
  searchAlgorithm( this->xRange ){
  try {
    const auto xWidth = ranges::cpp20::distance( this->xRange );
    if ( xWidth == 0 ){
      Log::error( "Grid of x-value has zero entries" );
      throw std::runtime_error("Grid of x-value has zero entries");
    }

    const auto yWidth = ranges::cpp20::distance( this->yRange );
    if ( xWidth != yWidth ){
      Log::error( "Inconsistent lengths of x-value and y-value grids" );
      Log::info( "Length of x-value grid: {}", xWidth );
      Log::info( "Length of y-value grid: {}", yWidth );
      throw std::runtime_error("Inconsistent lengths of x-value and y-value grids");
    }

    if ( not ranges::cpp20::is_sorted( this->xRange ) ){
      const auto xIterator = ranges::cpp20::is_sorted_until( this->xRange );
      const auto position  = ranges::cpp20::distance( this->xRange.begin(), xIterator );
      Log::error( "Unsorted values encountered in x-value grid" );
      Log::info( "Position of out-of-order value: {}", position );
      Log::info( "X-value[ {} ] : {}", position - 1, *ranges::cpp20::prev( xIterator ) );
      Log::info( "X-value[ {} ] : {}", position, *xIterator );
      throw std::runtime_error("Unsorted values encountered in x-value grid");
    }

    this->interpolant().verifyXGridAssumptions( this->xRange );
    this->interpolant().verifyYGridAssumptions( this->yRange );

  } catch ( std::exception& e ) {
    Log::info( "Error while constructing interpolation table" );
    throw e;
  }
}

Type( const Type& other ) :
  xRange( other.xRange ),
  yRange( other.yRange ),
  searchAlgorithm( this->xRange ){}

Type( Type&& other ) :
  xRange( std::move(other.xRange) ),
  yRange( std::move(other.yRange) ),
  searchAlgorithm( this->xRange ){}

Type& operator=( const Type& other ){
  this->xRange = other.xRange;
  this->yRange = other.yRange;
  this->searchAlgorithm = SearchAlgorithm{ this->xRange };
  return *this;
}

Type& operator=( Type&& other ){
  this->xRange = std::move(other.xRange);
  this->yRange = std::move(other.yRange);
  this->searchAlgorithm = SearchAlgorithm{ this->xRange };
  return *this;
}


~Type() = default;
