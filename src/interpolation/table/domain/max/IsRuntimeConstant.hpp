struct IsRuntimeConstant {

  template< typename Parent >
  class Type : public Max<Parent> {
  public:
    using Xdata = typename Parent::Xdata;

  protected:
    Xdata value;

  public:
    Xdata domainMax() const { return this->value; }
    
    template< typename... Args >
    Type( Xdata data, Args&&... args ) :
      Max<Parent>( std::forward< Args >( args )... ),
      value( data ){}
  };
  
};
