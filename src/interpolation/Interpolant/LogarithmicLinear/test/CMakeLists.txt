add_executable( interpolation.Interpolant.LogarithmicLinear.test
                apply.test.cpp
                invert.test.cpp
                LogarithmicLinear.test.cpp )
set( test_flags ${interpolation_compiler_flags} )
separate_arguments( test_flags_list UNIX_COMMAND "${test_flags}" )
foreach( flag IN LISTS test_flags_list )
    target_compile_options( interpolation.Interpolant.LogarithmicLinear.test PUBLIC ${flag} )
endforeach( flag )
set_target_properties( interpolation.Interpolant.LogarithmicLinear.test PROPERTIES LINK_FLAGS "${test_flags}" )
target_link_libraries( interpolation.Interpolant.LogarithmicLinear.test PUBLIC interpolation )
add_test( NAME interpolation.Interpolant.LogarithmicLinear COMMAND interpolation.Interpolant.LogarithmicLinear.test ) 
