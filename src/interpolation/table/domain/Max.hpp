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

public:
  static constexpr auto specifiesDomainMax(){ return true; }
  
  template< typename... Args >
  Max( Args&&... args ) : Parent( std::forward< Args >( args )... ){}
};

namespace max {

#include "interpolation/table/domain/max/IsRuntimeConstant.hpp"
#include "interpolation/table/domain/max/IsCompiletimeConstant.hpp"

}
