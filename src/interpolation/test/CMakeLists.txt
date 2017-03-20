add_executable( interpolation.test
                Example1.cpp 
                Example2.cpp 
                Example3.cpp 
                Example4.cpp 
                Example5.cpp 
                Example6.cpp 
                Example7.cpp 
                Example8.cpp 
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
