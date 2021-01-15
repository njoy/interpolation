struct LogarithmicLinear : public Interpolant {

  template< typename Xarg, typename X, typename Y >
  static auto apply( Xarg&& x, X&& xLeft, X&& xRight, Y&& yLeft, Y&& yRight ){
    using safe = std::decay_t<X>;
    return yLeft * std::pow( ( yRight / yLeft ),
			     ( safe(x) - xLeft ) / ( xRight - xLeft ) );
  }

  template< typename Yarg, typename X, typename Y >
  static auto invert( Yarg&& y, X&& xLeft, X&& xRight, Y&& yLeft, Y&& yRight ){
    using safe = std::decay_t<Y>;
    return
      ( yLeft != yRight ) ?
      xLeft + (xRight - xLeft) * std::log( safe(y) / yLeft )
                                 / std::log( yRight / yLeft ) :
      xLeft;
  }

  template< typename Xarg, typename X, typename Y >
  static auto integrate( Xarg&& xLow, Xarg&& xHi, X&& xLeft, X&& xRight, Y&& yLeft, Y&& yRight ){
    using safe = std::decay_t<X>;
    const auto base = yRight / yLeft;
    const auto denominator = std::log(base);
    const auto coefficient = yLeft * (xRight - xLeft);
    const auto exponent_hi = (xLeft - safe(xHi)) / (xLeft - xRight);
    const auto exponent_low = (xLeft - safe(xLow)) / (xLeft - xRight);
    return (coefficient / denominator) * 
           (std::pow(base, exponent_hi) - std::pow(base, exponent_low));
  }

  template< typename Range >
  static void verifyYGridAssumptions( Range&& range ){
    auto it = Interpolant::findChangeOfSign( range );
    if ( it != range.end() ){ 
      Log::error( "Encountered zero in function value of logarithmic-linear interpolation region" );
      Log::info( "Logarithmic-linear interpolation describes functions of the form: \n"
                 " y = a * b^(c * x)" );
      Log::info( "Functions of this form do not admit a change in sign" );
      if ( it == ranges::prev(range.end()) ){ it = ranges::prev(it); }
      const auto position = std::distance( range.begin(), it );
      Log::info(  "Y-value[ {} ]: {}", position, *it );
      Log::info(  "Y-value[ {} ]: {}", position + 1, *(++it) );
      throw std::logic_error( "Encountered zero in function value of logarithmic-linear interpolation region" );
    }
  }
};
