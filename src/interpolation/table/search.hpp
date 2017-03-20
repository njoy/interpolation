/* Question: 
 * Why is this dummy defining search algorithms as specializations of of a 
 * template
 *
 * Answer:
 * Defining Search algorithms as template specialization allows for downstream
 * templates to be easily contrained by pattern matching on the Search template
 */

template< typename... Ts >
struct Search;

#include "interpolation/table/search/Binary.hpp"
#include "interpolation/table/search/FromLeft.hpp"
#include "interpolation/table/search/FromRight.hpp"
#include "interpolation/table/search/Cached.hpp"
#include "interpolation/table/search/Hashed.hpp"

namespace search {

/* Question: 
 * Why is this dummy aliasing the templates he JUST defined with names that are
 * really similar to the tags?
 *
 * Answer:
 * The intent is to cleanup nested 'angley' brackets in the context they are used.
 */

using Binary = Search<Binary>;
using FromLeft = Search<FromLeft>;
using FromRight = Search<FromRight>;

template< typename Tag >
using Hashed = Search< Hashed<Tag> >;

}
