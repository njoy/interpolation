add_executable( interpolation.Interpolant.LogarithmicLogarithmic.test
                interpolate.test.cpp
                invert.test.cpp
                LogarithmicLogarithmic.test.cpp
                verifyXGridAssumptions.test.cpp
                verifyYGridAssumptions.test.cpp )
set( test_flags ${interpolation_compiler_flags} )
separate_arguments( test_flags_list UNIX_COMMAND "${test_flags}" )
foreach( flag IN LISTS test_flags_list )
    target_compile_options( interpolation.Interpolant.LogarithmicLogarithmic.test PUBLIC ${flag} )
endforeach( flag )
set_target_properties( interpolation.Interpolant.LogarithmicLogarithmic.test PROPERTIES LINK_FLAGS "${test_flags}" )
target_link_libraries( interpolation.Interpolant.LogarithmicLogarithmic.test PUBLIC interpolation )
add_test( NAME interpolation.Interpolant.LogarithmicLogarithmic COMMAND interpolation.Interpolant.LogarithmicLogarithmic.test ) 
