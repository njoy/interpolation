/* Question: 
 * Why is this dummy defining search algorithms as specializations of of a 
 * template
 *
 * Answer:
 * Defining Search algorithms as template specialization allows for downstream
 * templates to be easily contrained by pattern matching on the Discontinuity 
 * template
 */

template< typename... Ts >
struct Discontinuity;

template<>
struct Discontinuity<TakeLeft> {
  template< typename Data >
  static constexpr Data apply( const Data yLeft, const Data ){ return yLeft; }
};

template<>
struct Discontinuity<TakeRight> {
  template< typename Data >
  static constexpr Data apply( const Data, const Data yRight ){ return yRight; }
};

template<>
struct Discontinuity<TakeMean> {  
  template< typename Data >
  static constexpr Data apply( const Data yLeft, const Data yRight ){
    return 0.5 * (yLeft + yRight);
  }
};

namespace discontinuity{

using TakeLeft = Discontinuity<TakeLeft>;

using TakeRight = Discontinuity<TakeRight>;

using TakeMean = Discontinuity<TakeMean>;

}
