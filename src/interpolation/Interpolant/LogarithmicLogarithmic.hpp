struct LogarithmicLogarithmic : public Interpolant {
  
  template< typename Xarg, typename X, typename Y >
  static auto apply( Xarg&& x, X&& xLeft, X&& xRight, Y&& yLeft, Y&& yRight ){
    using safe = std::decay_t<X>;
    const auto inverseXLeft = 1.0 / xLeft;
    const auto logRatio = std::log( yRight / yLeft )
                          / std::log( xRight * inverseXLeft );
    return yLeft * std::pow( safe(x) * inverseXLeft, logRatio );
  }

  template< typename Yarg, typename X, typename Y >
  static auto invert( Yarg&& y, X&& xLeft, X&& xRight, Y&& yLeft, Y&& yRight ){
    using safe = std::decay_t<Y>;
    const auto inverseYLeft = 1.0 / yLeft;
    const auto logRatio = std::log( safe(y) * inverseYLeft )
                          / std::log( yRight * inverseYLeft );
    return xLeft * std::pow(xRight / xLeft, logRatio);
  }

  template< typename Range >
  static void verifyXGridAssumptions( Range&& range ){
    const auto zero = 0 * range.front();
    auto it = ( range.front() != zero ) ?
      Interpolant::findChangeOfSign( range ) : range.begin();
    if ( it != range.end() ){ 
      Log::error( "Encountered change in sign in x-value grid of logarithmic-logarithmic interpolation region" );
      Log::info( "A change of sign implies a function value at x == 0, \n"
                 "which is not defined under this scheme" );
      if ( it == ranges::prev( range.end() ) ){ it = ranges::prev(it); }
      const auto position = std::distance( range.begin(), it );
      Log::info(  "X-value[ {} ]: {}", position, *it );
      Log::info(  "X-value[ {} ]: {}", position + 1, *std::next(it) );
      throw std::logic_error( "Encountered zero in function value of logarithmic-logarithmic interpolation region" );
    }
  }

  template< typename Range >
  static void verifyYGridAssumptions( Range&& range ){
    const auto zero = 0 * range.front();
    auto it = ( range.front() != zero ) ?
      Interpolant::findChangeOfSign( range ) : range.begin();
    if ( it != range.end() ){ 
      Log::error( "Encountered change in sign in y-value grid of logarithmic-logarithmic interpolation region" );
      if ( it == ranges::prev( range.end() ) ){ it = ranges::prev(it); }
      const auto position = std::distance( range.begin(), it );
      Log::info( "Y-value[ {} ]: {}", position, *it );
      Log::info(  "Y-value[ {} ]: {}", position + 1, *std::next(it) );
      throw std::logic_error( "Encountered zero in function value of logarithmic-logarithmic interpolation region" );
    }
  }
};
