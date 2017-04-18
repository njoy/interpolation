
add_executable( interpolation.table.left.interval.IsCompiletimeConstant.test IsCompiletimeConstant.test.cpp )
target_compile_options( interpolation.table.left.interval.IsCompiletimeConstant.test PRIVATE ${${PREFIX}_common_flags}
$<$<BOOL:${strict}>:${${PREFIX}_strict_flags}>$<$<CONFIG:DEBUG>:
${${PREFIX}_DEBUG_flags}
$<$<BOOL:${coverage}>:${${PREFIX}_coverage_flags}>>
$<$<CONFIG:RELEASE>:
${${PREFIX}_RELEASE_flags}
$<$<BOOL:${link_time_optimization}>:${${PREFIX}_link_time_optimization_flags}>
$<$<BOOL:${nonportable_optimization}>:${${PREFIX}_nonportable_optimization_flags}>>

${CXX_appended_flags} ${interpolation_appended_flags} )
target_link_libraries( interpolation.table.left.interval.IsCompiletimeConstant.test PUBLIC interpolation ) 
add_test( NAME interpolation.table.left.interval.IsCompiletimeConstant COMMAND interpolation.table.left.interval.IsCompiletimeConstant.test )