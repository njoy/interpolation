struct LinearLogarithmic : public Interpolant {
  
  template< typename Xarg, typename X, typename Y >
  static auto apply( Xarg&& x, X&& xLeft, X&& xRight, Y&& yLeft, Y&& yRight ){
    using safe = std::decay_t<X>;
    const auto inverseXLeft = 1.0 / xLeft;
    return yLeft + ( yRight - yLeft )
      * std::log( safe(x) * inverseXLeft ) / std::log( xRight * inverseXLeft );
  }
  
  template< typename Yarg, typename X, typename Y >
  static auto invert( Yarg&& y, X&& xLeft, X&& xRight, Y&& yLeft, Y&& yRight ){
    using safe = std::decay_t<Y>;
    return ( yRight != yLeft ) ? 
      xLeft * std::pow( ( xRight / xLeft ),
                        ( ( safe(y) - yLeft ) / ( yRight - yLeft ) ) ) : xLeft;
  }

  template< typename Xarg, typename X, typename Y >
  static auto integrate( Xarg&& xLow, Xarg&& xHi, X&& xLeft, X&& xRight, Y&& yLeft, Y&& yRight ){
    using safe = std::decay_t<X>;
    const auto numerator_hi = xHi * ((yRight - yLeft) * std::log(safe(xHi) / xLeft) +
                              yLeft * std::log(xRight / xLeft) + yLeft - yRight);
    const auto numerator_low = xLow * ((yRight - yLeft) * std::log(safe(xLow) / xLeft) +
                               yLeft * std::log(xRight / xLeft) + yLeft - yRight);
    const auto denominator = std::log(xRight / xLeft);
    return (numerator_hi / denominator) - (numerator_low / denominator);
  }

  template< typename Range >
  static void verifyXGridAssumptions( Range&& range ){
    const auto zero = 0 * range.front();
    auto it =
      ( range.front() != zero ) ? findChangeOfSign( range ) : range.begin();
    if ( it != range.end() ){ 
      Log::error( "Encountered change in sign in x-value grid of linear-logarithmic interpolation region" );
      Log::info( "Linear-logarithmic interpolation describes functions of the form: \n"
                 " y = a * log(c * x), for x > 0 \n"
                 " y = a * log(c * -x), for x < 0" );
      Log::info( "A change of sign implies a function value at x == 0, \n"
                 "which is not defined under this scheme" );
      if ( it == ranges::prev( range.end() ) ){ it = ranges::prev(it); }
      const auto position = std::distance( range.begin(), it );
      Log::info( "X-value[ {} ]: {}", position, *it );
      Log::info( "X-value[ {} ]: {}", position + 1, *(++it) );
      throw std::logic_error( "Encountered zero in function value of linear-logarithmic interpolation region" );
    }
  }
};
