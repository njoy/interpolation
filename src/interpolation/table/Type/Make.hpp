template< typename Interpolant,
	  typename SearchTag,
	  typename DiscontinuityTag,
	  typename... TypeGenerators >
class Make< Interpolant,
	    Search< SearchTag >,
	    Discontinuity< DiscontinuityTag >,
	    TypeGenerators... >{

  template< typename... Ts >
  struct Parameters;

  template< typename T1, typename T2 >
  struct Parameters< T1, T2 >{
    using Xrange = std::decay_t<T1>;
    using Yrange = std::decay_t<T2>;
  };

  template< typename T1, typename T2, typename... Ts >
  struct Parameters< T1, T2, Ts... > : public Parameters< T2, Ts... >{};

public:
  template< typename... Args >
  auto operator()( Args&&... args ) const {
    return Table< Type< Interpolant,
			Search< SearchTag >,
			Discontinuity< DiscontinuityTag >,
			typename Parameters<Args...>::Xrange,
			typename Parameters<Args...>::Yrange >,
		  TypeGenerators... >( std::forward<Args>(args)... );
  }
};

template< typename Interpolant,
	  typename SearchTag,
	  typename DiscontinuityTag,
	  typename... TypeGenerators >
class Make< Interpolant,
	    Discontinuity< DiscontinuityTag >,
	    Search< SearchTag >,
	    TypeGenerators... > : public Make< Interpolant,
					       Search< SearchTag >,
					       Discontinuity<DiscontinuityTag>,
					       TypeGenerators... >{};

template< typename Interpolant,
	  typename SearchTag,
	  typename... TypeGenerators >
class Make< Interpolant,
	    Search< SearchTag >,
	    TypeGenerators... > : public Make< Interpolant,
					       Search< SearchTag >,
					       Discontinuity<TakeLeft>,
					       TypeGenerators... >{};

template< typename Interpolant,
	  typename DiscontinuityTag,
	  typename... TypeGenerators >
class Make< Interpolant,
	    Discontinuity<DiscontinuityTag>,
	    TypeGenerators... > : public Make< Interpolant,
					       Search< Binary >,
					       Discontinuity<DiscontinuityTag>,
					       TypeGenerators... >{};

template< typename Interpolant, typename... TypeGenerators >
class Make< Interpolant,
	    TypeGenerators... > : public Make< Interpolant,
					       Search< Binary >,
					       Discontinuity<TakeLeft>,
					       TypeGenerators... >{};
