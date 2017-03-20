template< typename... Args >
struct Implementation;

template< typename T >
struct Implementation< T > { using Type = T; };

template
< typename Base, typename TypeGenerator, typename... OtherTypeGenerators >
struct Implementation< Base, TypeGenerator, OtherTypeGenerators... >{
protected:
  using ParentType = typename Implementation< Base, OtherTypeGenerators... >::Type;
public:
  using Type = typename TypeGenerator::template Type< ParentType >;
};
