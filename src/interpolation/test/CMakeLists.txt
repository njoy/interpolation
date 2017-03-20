add_executable( interpolation.test
                #benchmark.cpp
                demo.cpp )
set( test_flags ${interpolation_compiler_flags} )
separate_arguments( test_flags_list UNIX_COMMAND "${test_flags}" )
foreach( flag IN LISTS test_flags_list )
    target_compile_options( interpolation.test PUBLIC ${flag} )
endforeach( flag )
set_target_properties( interpolation.test PROPERTIES LINK_FLAGS "${test_flags}" )
target_link_libraries( interpolation.test PUBLIC interpolation )
add_test( NAME interpolation COMMAND interpolation.test ) 
