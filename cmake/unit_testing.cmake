#######################################################################
# Setup
#######################################################################

message( STATUS "Adding interpolation unit testing" )
enable_testing()


#######################################################################
# Unit testing directories
#######################################################################

add_subdirectory( src/interpolation/Interpolant/Histogram/test )
add_subdirectory( src/interpolation/Interpolant/LinearLinear/test )
add_subdirectory( src/interpolation/Interpolant/LinearLogarithmic/test )
add_subdirectory( src/interpolation/Interpolant/LogarithmicLinear/test )
add_subdirectory( src/interpolation/Interpolant/LogarithmicLogarithmic/test )
add_subdirectory( src/interpolation/table/Table/test )
add_subdirectory( src/interpolation/table/Type/test )
add_subdirectory( src/interpolation/table/Variant/Iterator/helper/test )
add_subdirectory( src/interpolation/table/Variant/Iterator/test )
add_subdirectory( src/interpolation/table/Variant/test )
add_subdirectory( src/interpolation/table/Vector/test )
add_subdirectory( src/interpolation/table/domain/max/IsCompiletimeConstant/test )
add_subdirectory( src/interpolation/table/domain/max/IsRuntimeConstant/test )
add_subdirectory( src/interpolation/table/domain/min/IsCompiletimeConstant/test )
add_subdirectory( src/interpolation/table/domain/min/IsRuntimeConstant/test )
add_subdirectory( src/interpolation/table/left/interval/IsAsymptotic/test )
add_subdirectory( src/interpolation/table/left/interval/IsCompiletimeConstant/test )
add_subdirectory( src/interpolation/table/left/interval/IsRuntimeConstant/test )
add_subdirectory( src/interpolation/table/left/interval/Throws/test )
add_subdirectory( src/interpolation/table/make/test )
add_subdirectory( src/interpolation/table/right/interval/IsAsymptotic/test )
add_subdirectory( src/interpolation/table/right/interval/IsCompiletimeConstant/test )
add_subdirectory( src/interpolation/table/right/interval/IsRuntimeConstant/test )
add_subdirectory( src/interpolation/table/right/interval/Throws/test )
add_subdirectory( src/interpolation/table/search/test )
add_subdirectory( src/interpolation/table/transform/test )
add_subdirectory( src/interpolation/test )
