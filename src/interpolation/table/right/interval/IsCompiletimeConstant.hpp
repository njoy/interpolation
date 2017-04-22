template< typename Static >
struct IsCompiletimeConstant {

  template< typename Parent >
  class Type : public Interval<Parent> {
  public:
    using Ydata = typename Parent::Ydata;

    static_assert( Comparable< Ydata, decltype( Static::value ) >::value,
		   "Compile time constant right interval value type must be\n"
		   "comparable with parent classes y-data");

    static constexpr Ydata rightIntervalValue(){ return Static::value; }
    
    template< typename... Args >
    Type( Args&&... args ) :
      Interval< Parent >( std::forward< Args >( args )... ){}
  };
  
};
