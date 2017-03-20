template< typename... Args >
class Table;

namespace table {

struct Tag{};

template< typename... Args > class Make;
#include "interpolation/table/Implementation.hpp"

#include "interpolation/table/search.hpp"
#include "interpolation/table/discontinuity.hpp"
#include "interpolation/table/Type.hpp"

#include "interpolation/table/domain.hpp"
#include "interpolation/table/left.hpp"
#include "interpolation/table/right.hpp"
#include "interpolation/table/transform.hpp"

#include "interpolation/table/Variant.hpp"
#include "interpolation/table/Vector.hpp"

template< typename... Args >
constexpr Make< Args... > make{};

}

