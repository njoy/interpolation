add_executable( interpolation.table.Variant.Iterator.helper.test
                comparable.test.cpp
                diffable.test.cpp
                equitable.test.cpp
                helper.test.cpp )
set( test_flags ${interpolation_compiler_flags} )
separate_arguments( test_flags_list UNIX_COMMAND "${test_flags}" )
foreach( flag IN LISTS test_flags_list )
    target_compile_options( interpolation.table.Variant.Iterator.helper.test PUBLIC ${flag} )
endforeach( flag )
set_target_properties( interpolation.table.Variant.Iterator.helper.test PROPERTIES LINK_FLAGS "${test_flags}" )
target_link_libraries( interpolation.table.Variant.Iterator.helper.test PUBLIC interpolation )
add_test( NAME interpolation.table.Variant.Iterator.helper COMMAND interpolation.table.Variant.Iterator.helper.test ) 
