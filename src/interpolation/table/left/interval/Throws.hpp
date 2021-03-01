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

    template< typename Table, typename Arg, typename... Args >
    auto integrate( Arg&& xLow, Arg&& xHi, Args&&... args ) const {
      // Captures child leftLimit method
      const auto& table = static_cast< const Table& >( *this );
      if ( xLow < table.tableMin() ){
        Log::error("Lower integration limit in interval with undefined behavior.");
        Log::info("Lower integration limit: {}", xLow);
        Log::info("The function is undefined for values less than: {}",
                  table.tableMin() );
        throw std::out_of_range(
                "Lower integration limit in interval with undefined behavior." );
      }

      if ( xHi < table.tableMin() ){
        Log::error("Upper integration limit in interval with undefined behavior.");
        Log::info("Upper integration limit: {}", xHi);
        Log::info("The function is undefined for values less than: {}",
                  table.tableMin() );
        throw std::out_of_range(
                "Upper integration limit in interval with undefined behavior." );
      }

      return Parent:: template integrate<Parent>( std::forward<Arg>(xLow),
                                                  std::forward<Arg>(xHi),
                                                  std::forward<Args>(args)... );
    }
    
  public:
    template< typename... Args >
    Type( Args&&... args ) : Interval<Parent>( std::forward< Args >( args )... ){}
  };
  
};
