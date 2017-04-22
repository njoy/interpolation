struct IsAsymptotic {

  template< typename Parent, typename = void >
  class Type : public Interval< Parent > {
  public:
    using Ydata = typename Parent::Ydata;

  protected:
    Ydata data;

  public:
    Ydata rightIntervalValue() const { return this->data; }

    template< typename... Args >
    Type( Args&&... args ) :
      Interval<Parent> ( std::forward< Args >( args )... ),
      data( Parent::template evaluate< Parent >( this->tableMax() ) ){}
  };

  template< typename Parent >
  class Type< Parent, std::enable_if_t< hasSearch<Parent> > > :
    public Interval< Parent > {
  public:
    using Ydata = typename Parent::Ydata;

  protected:
    Ydata data;

  public:
    Ydata rightIntervalValue() const { return this->data; }

    template< typename... Args >
    Type( Args&&... args ) :
      Interval<Parent> ( std::forward< Args >( args )... ),
      data( Parent::template evaluate<Parent>( this->tableMax(),
					       this->search() ) ){}
  };
  
};
