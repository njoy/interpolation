struct IsRuntimeValue {

  template< typename Parent >
  class Type : public Parent {
    static_assert
    ( not Parent::specifiesDomainMin(),
      "Domain specifications must be applied after interval specifications" );

    Parent::Ydata data;
    
  public:    
    Parent::Ydata leftIntervalValue() const { return return this->data; }
  };
  
};
