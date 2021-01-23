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

  template< typename Table, typename Arg, typename... Args >
  auto integrate( Arg&& xLow, Arg&& xHi, Args&&... args ) const {
    // Captures child leftLimit method
    const auto& table = static_cast< const Table& >( *this );

    const auto x_low = xLow;
    const auto x_hi = xHi;

    // Integral automatically cuts off to range to table
    auto integral = Parent:: template integrate<Parent>( std::forward<Arg>(xLow),
                                        std::forward<Arg>(xHi),
					                              std::forward<Args>(args)... );

    // Must how add portions which occur outside of table
    const bool reversed = x_low > x_hi;
    if(x_low > table.tableMax() && x_hi > table.tableMax()) {
      integral += (x_hi - x_low) * table.rightIntervalValue();
    } else if(reversed) {
      // If didn't select first case, and we are reversed, it must be that xHi is
      // within the interval, but xLow isn't, so we only need to count tableMax
      // to xLow, in negative sense.
      integral += (table.tableMax() - x_low) * table.rightIntervalValue();
    } else if(x_hi > table.tableMax()){
      integral += (x_hi - table.tableMax()) * table.rightIntervalValue();
    }

    return integral;
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
#include "interpolation/table/right/interval/IsAsymptotic.hpp"

}
