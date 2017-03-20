struct LinearLinear : public Interpolant {
  template< typename Xarg, typename X, typename Y >
  static auto apply( Xarg&& x, X&& xLeft, X&& xRight, Y&& yLeft, Y&& yRight ){
    return yLeft + ( yRight - yLeft) * ( x - xLeft ) / ( xRight - xLeft );
  }

  template< typename Yarg, typename X, typename Y >
  static auto invert( Yarg&& y, X&& xLeft, X&& xRight, Y&& yLeft, Y&& yRight ){
    return xLeft + ( xRight - xLeft ) * ( y - yLeft ) / ( yRight - yLeft );
  }
};
