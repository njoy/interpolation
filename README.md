![Continuous Integration](https://github.com/njoy/interpolation/workflows/Continuous%20Integration/badge.svg)

njoy::interpolation
=========

`interpolation` is a header-only C++ library providing generic classes for interpolation
of one dimensional data.

Representative Examples
----------------

### Example 1: A simple table

The interpolation library provides configurable interpolation table types.
Where possible, helper functions provide type deduction to reduce verbosity.

```c++
#include <iostream>
#include <vector>

#include "interpolation.hpp"

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

### Example 2: Defaulted behavior

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

### Example 3: Cached Lookup

There sometimes arise occasions in which subsequent table evaluations interpolate
within the same grid bin. Alternatively, it might be useful to evaluate multiple
tables tabulated upon the same independent value grid. In these cases, repeatedly
searching the table would be wasteful. The address this redunancy, many tables
provide a `cachedSearch` method. This method returns an object which can be passed
as a second argument during interpolation calls to eliminate unnecessary searches,
provided subsequent searches operate on a shared x-grid.

```c++
#include <iostream>
#include <vector>

#include "interpolation.hpp"

using namespace njoy::interpolation;

int main(){

  auto viewOf = []( const auto& range ){
    return ranges::make_iterator_range( range.begin(), range.end() );
  };

  std::vector< double > xGrid  = { 1.0, 2.0, 3.0, 4.0,  5.0 };
  std::vector< double > y1Grid = { 3.0, 5.0, 7.0, 9.0, 11.0 };
  std::vector< double > y2Grid = { 9.0, 7.0, 3.0, 5.0,  7.0 };

  auto partialXGrid = viewOf(xGrid) | ranges::view::drop_exactly(2);
  std::vector< double > y3Grid = { 7.0, 5.0, 3.0 };

  auto table1 = table::make< LinearLinear >( viewOf(xGrid), std::move(y1Grid) );
  auto table2 = table::make< LinearLinear >( viewOf(xGrid), std::move(y2Grid) );
  auto table3 =
    table::make< LinearLinear >( std::move(partialXGrid), std::move(y3Grid) );

  auto search = table1.cachedSearch();
  std::cout << table1( 3.5, search ) << std::endl;
  std::cout << table1( 3.75, search ) << std::endl; // cached search

  std::cout << table2( 3.5, search ) << std::endl; // also cached search

  std::cout << table3( 3.5, search ) << std::endl; // also cached search
  std::cout << table3( 4.5, search ) << std::endl; // new search

  std::cout << table1( 4.75, search ) << std::endl; // also cached
}  
```

Output

```
  8
  8.5
  4
  6
  4
  10.5
```

### Example 4: Hashed Lookup

For strictly positive independent value grids, table search performance may
be dramatically improved using a hash-based lookup.

```c++
  /* given the setup code in Example 1 */

  /* As in example 1, the search method is specified. In this example,
   * a hash function is used to narrow the searched space, which is then
   * refined to a single bin using binary search.
   */
  auto myTable =
  table::make< LinearLinear,
               table::search::Hashed<Binary> >( std::move(xGrid),
                                                std::move(yGrid) );

  std::cout << myTable( 2.5 ) << std::endl;
```

Output

```
6
```

### Example 5: Generality

Interpolation tables are generic with respect to data and range types.
In this example, we leverage data types provided by the dimwits library and
range types provided by the Range V3 library.

```c++
#include "dimwits.hpp"

  /* given the setup code in Example 1 */

  using namespace dimwits;

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

### Example 6: Decoration

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
                 table::domain::min::IsRuntimeConstant,
                 table::domain::max::IsCompiletimeConstant<Ten>,
                 table::left::interval::IsCompiletimeConstant<Zero>,
                 table::right::interval::Throws >( 0.0, // domain min
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

### Example 7: Variants

The interpolation library provides an efficient `Variant` class template.
The `Variant` provides the same interface as its parameter types but does not
incur virtualization and heap allocation overhead.

```c++
  /* given the setup code in Example 1 */

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

### Example 8: Vectors

The inteperpolation library provides a composite table called a `Vector`.
This is useful when used in conjunction with the `Variant` to describe
a range of values over which the interpolation scheme varies.

```c++
#include <iostream>
#include <vector>

#include "interpolation.hpp"

using namespace njoy::interpolation;

int main(){

  std::vector< double > xGrid = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0 };
  std::vector< double > yGrid = { 3.0, 5.0, 7.0, 6.0, 2.0, 9.0, 8.0, 5.0, 1.0,  0.0 };

  using Partition =
    decltype( xGrid | ranges::view::drop_exactly(0) | ranges::view::take_exactly(0) );

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
  core.push_back( Law2( partition( xGrid, 0, 3 ), partition( yGrid, 0, 3 ) ) );
  core.push_back( Law5( partition( xGrid, 2, 4 ), partition( yGrid, 2, 4 ) ) );
  core.push_back( Law3( partition( xGrid, 5, 5 ), partition( yGrid, 5, 5 ) ) );
  Tab1 myTable( std::move(core) );

  std::cout << myTable( 2.5 ) << std::endl;
}
```

Output
```
6
```
