template< typename Static >
struct IsCompiletimeConstant {

  template< typename Parent >
  class Type : public Max<Parent> {
  public:
    using Xdata = typename Parent::Xdata;

    static_assert( Comparable< Xdata, decltype( Static::value ) >::value,
		   "Compile time constant right limit value type must be\n"
		   "comparable with parent classes x-data" );

    static constexpr Xdata domainMax(){ return Static::value; }
   
    template< typename... Args >
    Type( Args&&... args ) : Max<Parent>( std::forward< Args >( args )... ){}
  };
  
};
