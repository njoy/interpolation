add_executable( interpolation.Interpolant.LinearLogarithmic.test
                interpolate.test.cpp
                invert.test.cpp
                LinearLogarithmic.test.cpp
                verifyXGridAssumptions.test.cpp )
set( test_flags ${interpolation_compiler_flags} )
separate_arguments( test_flags_list UNIX_COMMAND "${test_flags}" )
foreach( flag IN LISTS test_flags_list )
    target_compile_options( interpolation.Interpolant.LinearLogarithmic.test PUBLIC ${flag} )
endforeach( flag )
set_target_properties( interpolation.Interpolant.LinearLogarithmic.test PROPERTIES LINK_FLAGS "${test_flags}" )
target_link_libraries( interpolation.Interpolant.LinearLogarithmic.test PUBLIC interpolation )
add_test( NAME interpolation.Interpolant.LinearLogarithmic COMMAND interpolation.Interpolant.LinearLogarithmic.test ) 
