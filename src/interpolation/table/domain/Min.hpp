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

public:
  static constexpr auto specifiesDomainMin(){ return true; }
  
  template< typename... Args >
  Min( Args&&... args ) : Parent( std::forward< Args >( args )... ){}
};

namespace min {

#include "interpolation/table/domain/min/IsRuntimeConstant.hpp"
#include "interpolation/table/domain/min/IsCompiletimeConstant.hpp"

}
