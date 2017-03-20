struct IsRuntimeConstant {

  template< typename Parent >
  class Type : public Min<Parent> {
  public:
    using Xdata = typename Parent::Xdata;

  protected:
    Xdata value;
    
  public:
    Xdata domainMin() const { return this->value; }
    
    template< typename... Args >
    Type( Xdata data, Args&&... args ) :
      Min<Parent>( std::forward< Args >( args )... ),
      value( data ){}
  };
  
};
