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
  auto integrate( Arg&& xL, Arg&& xH, Args&&... args ) const {
    // Captures child leftLimit method
    const auto& table = static_cast< const Table& >( *this );

    const auto xLow = xL;
    const auto xHi = xH;

    // Integral automatically cuts off to range to table
    auto integral = Parent:: template integrate<Parent>( std::forward<Arg>(xL),
                                        std::forward<Arg>(xH),
					                              std::forward<Args>(args)... );

    // Must how add portions which occur outside of table
    const bool reversed = xLow > xHi;
    if(xLow < table.tableMin() && xHi < table.tableMin()) {
      integral += (xHi - xLow) * table.leftIntervalValue();
    } else if(reversed) {
      // If didn't select first case, and we are reversed, it must be that xLow is
      // within the interval, but xHi isn't, so we only need to count tableMin
      // to xHi, in negative sense.
      integral += (xHi - table.tableMin()) * table.leftIntervalValue();
    } else if(xLow < table.tableMin()){
      integral += (table.tableMin() - xLow) * table.leftIntervalValue();
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
