#CmakeLists.txt fot util/shading/

# Add the utility for generating direct radiation 
add_executable(shade shading.c)

install(TARGETS shade 
    RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin)
