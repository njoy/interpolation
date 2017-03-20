template< typename Parent >
class Interval : public Parent {
  static_assert
  ( not Parent::specifiesDomainMax(),
    "Domain specifications must be applied after interval specifications" );

protected:
  template< typename Table, typename Arg, typename... Args >
  auto evaluate( Arg&& x, Args&&... args ) const {
    // Captures child rightIntervalValue method
    const auto& table = static_cast< const Table& >( *this );
    return ( x > table.tableMax() ) ?
      table.rightIntervalValue() :
      Parent::template evaluate<Parent>( std::forward<Arg>(x),
					 std::forward<Args>(args)... );
  }
  
public:
  static constexpr auto specifiesRightInterval(){ return true; }
  
  template< typename... Args >
  Interval( Args&&... args ) : Parent( std::forward< Args >( args )... ){}
};

namespace interval {

#include "interpolation/table/right/interval/Throws.hpp"
#include "interpolation/table/right/interval/IsRuntimeConstant.hpp"
#include "interpolation/table/right/interval/IsCompiletimeConstant.hpp"

}
