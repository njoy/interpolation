namespace variant {

#include "interpolation/table/Variant/helper.hpp"
#include "interpolation/table/Variant/Iterator.hpp"
#include "interpolation/table/Variant/Search.hpp"

}

template< typename TableType, typename... TableTypes >
class Variant {
  std::variant< TableType, TableTypes... > core;

  static_assert( variant::all( std::is_same< typename TableType::Xdata,
		                             typename TableTypes::Xdata >::value... ), "" );

  static_assert( variant::all( std::is_same< typename TableType::Ydata,
			                     typename TableTypes::Ydata >::value... ), "" );
  
public:
  using Xdata = typename TableType::Xdata;
  using Ydata = typename TableType::Ydata;
  
#define SIMPLE_VISIT_METHOD( name )				\
  auto name() const {						\
    return std::visit(						\
      []( auto&& table ){ return table.name(); }, this->core ); \
  }								\

  SIMPLE_VISIT_METHOD( specifiesLeftInterval )
  SIMPLE_VISIT_METHOD( specifiesRightInterval )
  SIMPLE_VISIT_METHOD( specifiesDomainMin )
  SIMPLE_VISIT_METHOD( specifiesDomainMax )
  SIMPLE_VISIT_METHOD( tableMin )
  SIMPLE_VISIT_METHOD( tableMax )
  SIMPLE_VISIT_METHOD( domainMin )
  SIMPLE_VISIT_METHOD( domainMax )
#undef SIMPLE_VISIT_METHOD

protected:
  using XLeadingIterator = decltype( std::declval<TableType>().x().begin() );
  using XVariantIterator =
    typename variant::Iterator< decltype( std::declval<TableType>().x().begin() ),
				decltype( std::declval<TableTypes>().x().begin() )... >::Type;

  static constexpr bool xDistinct =
    not std::is_same< XLeadingIterator, XVariantIterator >::value;

public:
  using Xrange = std::conditional_t
    < xDistinct,
      decltype( ranges::make_iterator_range
		( std::declval<XVariantIterator>(),
		  std::declval<XVariantIterator>() ) ),
      decltype( std::declval<TableType>().x() ) >;

protected:
  auto x( std::true_type ) const {
    return std::visit(
      []( auto&& core ){
	return ranges::make_iterator_range
	  ( XVariantIterator( core.x().begin() ),
	    XVariantIterator( core.x().end() ) );
      }, this->core );
  }

  const auto& x( std::false_type ) const {
    return std::visit(
      []( auto&& core ) -> decltype( std::declval<TableType>().x() ){
	return core.x();
      }, this->core );
  }
  
public:
  Xrange x() const {
    return this->x( std::integral_constant< bool, xDistinct >{} );
  }

protected:
  using YLeadingIterator = decltype( std::declval<TableType>().y().begin() );
  using YVariantIterator =
    typename variant::Iterator< decltype( std::declval<TableType>().y().begin() ),
				decltype( std::declval<TableTypes>().y().begin() )... >::Type;

  static constexpr bool yDistinct =
    not std::is_same< YLeadingIterator, YVariantIterator >::value;

  using Yrange = std::conditional_t
    < yDistinct,
      decltype( ranges::make_iterator_range
		( std::declval<YVariantIterator>(),
		  std::declval<YVariantIterator>() ) ),
      decltype( std::declval<TableType>().y() ) >;

  auto y( std::true_type ) const {
    return std::visit(
      []( auto&& core ){
	return ranges::make_iterator_range
	  ( YVariantIterator( core.y().begin() ),
	    YVariantIterator( core.y().end() ) );
      }, this->core );
  }

  const auto& y( std::false_type ) const {
    return std::visit(
      []( auto&& core ) -> decltype( std::declval<TableType>().y() ){
	return core.y();
      }, this->core );
  }

public:
  Yrange y() const {
    return this->y( std::integral_constant< bool, yDistinct >{} );
  }

protected:
  template< typename Table, typename Arg, typename... Args >
  auto evaluate( Arg&& x, Args&&... args ) const {
    return std::visit(
      [&]( auto&& core ){
	return core( std::forward<Arg>(x), std::forward<Args>(args)... );
      }, this->core );
  }

public:  
  template< typename Arg >
  Variant( Arg&& arg ) : core( std::forward< Arg >( arg ) ){}
  Variant( Variant& ) = default;
  Variant( Variant&& ) = default;
  Variant( const Variant& ) = default;
  
  template< typename Arg >
  Variant& operator=( Arg&& arg ){ this->core = std::forward< Arg >( arg ); }
  Variant& operator=( Variant& ) = default;
  Variant& operator=( Variant&& ) = default;
  Variant& operator=( const Variant& ) = default;
  
  template< typename... Args >
  friend struct Search;

  auto search() const {
    using Result = 
      typename variant::Search
      < decltype( std::declval<TableType>().search() ),
	decltype( std::declval<TableTypes>().search() )... >::Type;
    return std::visit( [&]( auto&& core ){ return Result(core.search()); },
		       this->core );
  }

  auto cachedSearch() const {
    using Result = 
      typename variant::Search
      < decltype( std::declval<TableType>().cachedSearch() ),
	decltype( std::declval<TableTypes>().cachedSearch() )... >::Type;
    return std::visit( [&]( auto&& core ){ return Result(core.cachedSearch()); },
		       this->core );
  }
};

#include "interpolation/table/Variant/Table.hpp"
