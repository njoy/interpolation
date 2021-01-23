template< typename Parent >
class Interval : public Parent {
  static_assert
  ( not Parent::specifiesDomainMin(),
    "Domain specifications must be applied after interval specifications" );

protected:
  template< typename Table, typename Arg, typename... Args >
  auto evaluate( Arg&& x, Args&&... args ) const {
    // Captures child leftIntervalValue method
    const auto& table = static_cast<const Table&>( *this );
    return ( x < table.tableMin() ) ?
      table.leftIntervalValue() :
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
    if(x_low < table.tableMin() && x_hi < table.tableMin()) {
      integral += (x_hi - x_low) * table.leftIntervalValue();
    } else if(reversed) {
      // If didn't select first case, and we are reversed, it must be that xLow is
      // within the interval, but xHi isn't, so we only need to count tableMin
      // to xHi, in negative sense.
      integral += (x_hi - table.tableMin()) * table.leftIntervalValue();
    } else if(x_low < table.tableMin()){
      integral += (table.tableMin() - x_low) * table.leftIntervalValue();
    }

    return integral;
  }
  
public:
  static constexpr auto specifiesLeftInterval(){ return true; }
  
  template< typename... Args >
  Interval( Args&&... args ) : Parent( std::forward< Args >( args )... ){}
};

namespace interval {

#include "interpolation/table/left/interval/Throws.hpp"
#include "interpolation/table/left/interval/IsRuntimeConstant.hpp"
#include "interpolation/table/left/interval/IsCompiletimeConstant.hpp"
#include "interpolation/table/left/interval/IsAsymptotic.hpp"

}
