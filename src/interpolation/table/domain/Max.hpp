template< typename Parent >
class Max : public Parent {
protected:
  template< typename Table, typename Arg, typename... Args >
  auto evaluate( Arg&& x, Args&&... args ) const {
    // Captures child rightLimit method
    const auto& table = static_cast<const Table&>( *this );
    if ( x > table.domainMax() ){
      Log::error( "Value queried is greater than right domain limit." );
      Log::info( "Queried value: {}", x );
      Log::info( "Right domain limit: {}", table.domainMax() );
      throw std::out_of_range( "Value queried is outside table domain." );
    }
    return Parent::template evaluate<Parent>( std::forward<Arg>(x),
					      std::forward<Args>(args)... );
  }

  template< typename Table, typename Arg, typename... Args >
  auto integrate( Arg&& xLow, Arg&& xHi, Args&&... args ) const {
    // Captures child leftLimit method
    const auto& table = static_cast< const Table& >( *this );
    if ( xLow > table.domainMax() ){
      Log::error( "Lower integration limit is greater than right domain limit." );
      Log::info( "Lower integration limit: {}", xLow );
      Log::info( "Right domain limit: {}", table.domainMax() );
      throw std::out_of_range("Lower integration limit is outside table domain.");
    }

    if ( xHi > table.domainMax() ){
      Log::error( "Upper integration limit is greater than right domain limit." );
      Log::info( "Uppper integration limit: {}", xHi );
      Log::info( "Right domain limit: {}", table.domainMax() );
      throw std::out_of_range("Upper integration limit is outside table domain.");
    }

    return Parent:: template integrate<Parent>( std::forward<Arg>(xLow),
                                                std::forward<Arg>(xHi),
					                                      std::forward<Args>(args)... );
  }

public:
  static constexpr auto specifiesDomainMax(){ return true; }
  
  template< typename... Args >
  Max( Args&&... args ) : Parent( std::forward< Args >( args )... ){}
};

namespace max {

#include "interpolation/table/domain/max/IsRuntimeConstant.hpp"
#include "interpolation/table/domain/max/IsCompiletimeConstant.hpp"

}
