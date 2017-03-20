template< typename Static >
struct IsCompiletimeConstant {

  template< typename Parent >
  class Type : public Min<Parent> {
  public:
    using Xdata = typename Parent::Xdata;
    static_assert( Comparable< Xdata, decltype( Static::value ) >::value,
		   "Compile time constant left limit value type must be\n"
		   "comparable with parent classes x-data" );

    static constexpr Xdata domainMin(){ return Static::value; }
   
    template< typename... Args >
    Type( Args&&... args ) : Min<Parent>( std::forward< Args >( args )... ){}
  };
  
};
