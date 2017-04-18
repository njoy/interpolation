
add_executable( interpolation.Interpolant.LinearLogarithmic.test
                interpolate.test.cpp
                invert.test.cpp
                LinearLogarithmic.test.cpp
                verifyXGridAssumptions.test.cpp )
target_compile_options( interpolation.Interpolant.LinearLogarithmic.test PRIVATE ${${PREFIX}_common_flags}
$<$<BOOL:${strict}>:${${PREFIX}_strict_flags}>$<$<CONFIG:DEBUG>:
${${PREFIX}_DEBUG_flags}
$<$<BOOL:${coverage}>:${${PREFIX}_coverage_flags}>>
$<$<CONFIG:RELEASE>:
${${PREFIX}_RELEASE_flags}
$<$<BOOL:${link_time_optimization}>:${${PREFIX}_link_time_optimization_flags}>
$<$<BOOL:${nonportable_optimization}>:${${PREFIX}_nonportable_optimization_flags}>>

${CXX_appended_flags} ${interpolation_appended_flags} )
target_link_libraries( interpolation.Interpolant.LinearLogarithmic.test PUBLIC interpolation ) 
add_test( NAME interpolation.Interpolant.LinearLogarithmic COMMAND interpolation.Interpolant.LinearLogarithmic.test )