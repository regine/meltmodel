# CMakeLists.txt for util/gridtools/


add_executable(ascigrid ascigrid.c)
add_executable(gridasci gridasci.c)

install(TARGETS ascigrid gridasci 
    RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin)
