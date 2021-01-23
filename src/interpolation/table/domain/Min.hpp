template< typename Parent >
class Min : public Parent {
protected:
  template< typename Table, typename Arg, typename... Args >
  auto evaluate( Arg&& x, Args&&... args ) const {
    // Captures child leftLimit method
    const auto& table = static_cast< const Table& >( *this );
    if ( x < table.domainMin() ){
      Log::error( "Value queried is less than left domain limit." );
      Log::info( "Queried value: {}", x );
      Log::info( "Left domain limit: {}", table.domainMin() );
      throw std::out_of_range( "Value queried is outside table domain." );
    }
    return Parent:: template evaluate<Parent>( std::forward<Arg>(x),
					       std::forward<Args>(args)... );
  }

  template< typename Table, typename Arg, typename... Args >
  auto integrate( Arg&& xLow, Arg&& xHi, Args&&... args ) const {
    // Captures child leftLimit method
    const auto& table = static_cast< const Table& >( *this );
    if ( xLow < table.domainMin() ){
      Log::error( "Lower integration limit is less than left domain limit." );
      Log::info( "Lower integration limit: {}", xLow );
      Log::info( "Left domain limit: {}", table.domainMin() );
      throw std::out_of_range("Lower integration limit is outside table domain.");
    }

    if ( xHi < table.domainMin() ){
      Log::error( "Upper integration limit is less than left domain limit." );
      Log::info( "Uppper integration limit: {}", xHi );
      Log::info( "Left domain limit: {}", table.domainMin() );
      throw std::out_of_range("Upper integration limit is outside table domain.");
    }

    return Parent:: template integrate<Parent>( std::forward<Arg>(xLow),
                                                std::forward<Arg>(xHi),
					                                      std::forward<Args>(args)... );
  }

public:
  static constexpr auto specifiesDomainMin(){ return true; }
  
  template< typename... Args >
  Min( Args&&... args ) : Parent( std::forward< Args >( args )... ){}
};

namespace min {

#include "interpolation/table/domain/min/IsRuntimeConstant.hpp"
#include "interpolation/table/domain/min/IsCompiletimeConstant.hpp"

}
