Interpolation
=========


Minimal Example
----------------

### Example 1

The interpolation library provides considerable configurable interpolation table types.
Where possible, helper methods provide type deduction to reduce verbosity.

```c++
#include <iostream>
#include <vector>

#include "dimwits.hpp"
#include "interpolation.hpp"

using namespace dimwits;
using namespace njoy::interpolation;

int main(){

  std::vector< double > xGrid = { 1.0, 2.0, 3.0 };
  std::vector< double > yGrid = { 3.0, 5.0, 7.0 };

  /* A linear-linear interpolation table using
   * + double for the x-value datatype
   * + vector<double> for the x-range
   * + double for the y-value datatype
   * + vector<double> for the y-range
   * + binary search to determine the appropriate bin 
   * + prefering the left argument given coincident points
   */
  auto myTable =
    table::make< LinearLinear,
                 table::search::Binary,
                 table::discontinuity::TakeLeft >( std::move(xGrid),
                                                   std::move(yGrid) );

  // Interpolation tables are function objects and are callable
  std::cout << myTable( 2.5 ) << std::endl;
  std::cout << myTable.domainMin() << ' ' << myTable.domainMax() << std::endl;
  std::cout << myTable.tableMin() << ' ' << myTable.tableMax() << std::endl;
}
```

Output

```
6
-Inf +Inf
1 3
```

### Example 2

Where reasonable, defaults are provided for common options

```c++
  /* given the setup code in Example 1 */

  /* The `make` helper provides default for common choices
   * + when a search policy is unspecified, binary search is used
   * + when a discontinuity policy is unspecified, left preference is used
   */
  auto myTable = table::make< LinearLinear >( std::move(xGrid), std::move(yGrid) );

  std::cout << myTable( 2.5 ) << std::endl;
```

Output

```
6
```

### Example 3

Interpolation tables are generic with respect to data and range types.
In this example, we leverage data types provided by the dimwits library and
range types provided by the Range V3 library.

```c++
  /* given the setup code in Example 1 */
  
  auto energy = xGrid
    | ranges::view::transform( []( auto&& arg ){ return arg * electronVolts; } );

  auto crossSection = yGrid
    | ranges::view::transform( []( auto&& arg ){ return arg * barns; } );

  auto myTable =
    table::make< LinearLinear >( std::move(energy), std::move(crossSection) );

  std::cout << myTable( 2.5 * electronVolts ) << std::endl;
  std::cout << myTable( 2.5E-3 * kilo(electronVolts) ) << std::endl;
```

Output

```
 6 b
 6 b
```

### Example 4

Interpolation tables can decorated to specify addition behavior.

```c++
struct Zero {
  static constexpr double value = 0.0;
};

struct Ten {
  static constexpr double value = 10.0;
};

  /* given the setup code in Example 1 */
  
  auto myTable =
    table::make< LinearLinear,
                 left::interval::IsCompiletimeConstant<Zero>,
                 right::interval::Throws,
                 domain::min::IsRuntimeConstant,
                 domain::max::IsCompiletimeConstant<Ten> >( 0.0, // domain min
                                                            std::move(xGrid),
                                                            std::move(yGrid) );

  std::cout << myTable( 2.5 ) << std::endl;
  std::cout << myTable.domainMin() << ' ' << myTable.domainMax() << std::endl;
  std::cout << myTable.tableMin() << ' ' << myTable.tableMax() << std::endl;

  std::cout << myTable( 0.5 ) << std::endl;
  try {
    myTable( -1.0 );
  } catch( std::exception& ){  std::cout << std::endl; }
  try {
    myTable( 3.5 );
  } catch( std::exception& ){ std::cout << std::endl; }
  try {
    myTable( 11.0 );
  } catch( std::exception& ){ std::cout << std::endl; }

```

Output
```
6
0 10
1 3
0
[error] Value queried is less than left domain limit.
[info] Queried value: -1
[info] Left domain limit: 0

[error] Value queried in interval with undefined behavior.
[info] Queried value: 3.5
[info] The function value is undefined for query values greater than 3.0

[error] Value queried is greater than right domain limit.
[info] Queried value: 11
[info] Right domain limit: 10
```

### Example 5

The inteperpolation library provides an efficient `Variant` class template.
The `Variant` provides the same interface as its parameter types but does not
incur virtualization and heap allocation overhead.

```c++
  /* given the setup code in Example 1 */

  /* The `make` helper provides default for common choices
   * + when a search policy is unspecified, binary search is used
   * + when a discontinuity policy is unspecified, left preference is used
   */
  using Law2 =
    decltype( table::make< LinearLinear >( std::move(xGrid), std::move(yGrid) ) );

  using Law5 =
    decltype( table::make< LogarithmicLogarithmic >( std::move(xGrid),
                                                     std::move(yGrid) ) );

  Table< table::Variant< Law2, Law5 > > myTable( Law2( std::move(xGrid),
                                                       std::move(yGrid) ) );

  std::cout << myTable( 2.5 ) << std::endl;
```

Output
```
6
```

### Example 6

The inteperpolation library provides a composite table called a `Vector`.
This is useful when used in conjunction with the `Variant` to describe
a range of values over which the interpolation scheme varies.

```c++
  std::vector< double > xGrid = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0 };
  std::vector< double > yGrid = { 3.0, 5.0, 7.0, 6.0, 2.0, 9.0, 8.0, 5.0, 1.0,  0.0 };

  using Partition =
    decltype( xGrid | ranges::view::drop_exactly(0) | ranges::view::take_exactly(0) );

  Table< table::Type<> > 

  using Law1 =
    decltype( table::make< Histogram >( std::declval<Partition>(),
                                        std::declval<Partition>() ) );

  using Law2 =
    decltype( table::make< LinearLinear >( std::declval<Partition>(),
                                           std::declval<Partition>() ) );

  using Law3 =
    decltype( table::make< LinearLogarithmic >( std::declval<Partition>(),
                                                std::declval<Partition>() ) );

  using Law4 =
    decltype( table::make< LogarithmicLinear >( std::declval<Partition>(),
                                                std::declval<Partition>() ) );

  using Law5 =
    decltype( table::make< LogarithmicLogarithmic >( std::declval<Partition>(),
                                                     std::declval<Partition>() ) );

  using ENDFvariant = Table< table::Variant< Law1, Law2, Law3, Law4, Law5 > >;

  using Tab1 = Table< table::Vector<ENDFvariant> >;

  auto partition = []( auto&& range, int drop, int take ){
    return range
           | ranges::view::drop_exactly(drop)
           | ranges::view::take_exactly(take);
  };

  std::vector< ENDFvariant > core;
  core.push_back( Law1( partition( xGrid, 0, 3 ), partition( yGrid, 0, 3 ) ) );
  core.push_back( Law5( partition( xGrid, 2, 4 ), partition( yGrid, 2, 4 ) ) );
  core.push_back( Law3( partition( xGrid, 5, 5 ), partition( yGrid, 5, 5 ) ) );
  Tab1 myTable( std::move(core) );

  std::cout << myTable( 2.5 ) << std::endl;
```

Output
```
6
```