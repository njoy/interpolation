struct IsRuntimeConstant {

  template< typename Parent >
  class Type : public Interval<Parent> {
  public:
    using Ydata = typename Parent::Ydata;
    
  protected:
    Ydata value;
    
  public:
    Ydata rightIntervalValue() const { return this->value; }

    template< typename... Args >
    Type( Ydata data, Args&&... args ) :
      Interval<Parent>( std::forward< Args >( args )... ), value( data ){}
  };
  
};
