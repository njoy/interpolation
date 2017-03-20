struct Throws {

  template< typename Parent >
  class Type : public Interval<Parent> {
  protected:
    template< typename Table, typename Arg, typename... Args >
    auto evaluate( Arg&& x, Args&&... args ) const {
      if ( x < this->tableMin() ){
	Log::error( "Value queried in interval with undefined behavior." );
        Log::info( "Queried value: {}", x );
        Log::info( "The function value is undefined for query values less than {}",
		   this->tableMin() );
        throw std::out_of_range( "Value queried in interval with undefined behavior." );
      }
      
      return Parent::template evaluate<Parent>( std::forward<Arg>(x),
						std::forward<Args>(args)... );
    }
    
  public:
    template< typename... Args >
    Type( Args&&... args ) : Interval<Parent>( std::forward< Args >( args )... ){}
  };
  
};
