struct IsRuntimeValue {

  template< typename Parent >
  class Type : public Parent {
    static_assert
    ( not Parent::specifiesDomainMax(),
      "Domain specifications must be applied after interval specifications" );

    Parent::Ydata data;
    
  public:    
    Parent::Ydata rightIntervalValue() const { return this->data; }
  };
  
};
