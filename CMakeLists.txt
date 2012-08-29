# CMakeLists.txt for the root of the meltmod project
# Tells cmake about the build environment and where to look
# for source files
cmake_minimum_required (VERSION 2.8)
project (MeltModel C)


#Compiler Flags 
#workaround for OSX Lion 64-bit seg-fault issue
if(APPLE)
    set(CMAKE_C_FLAGS "-m32")
endif(APPLE)
#LOTS of compiler warnings foe debug builds
set(CMAKE_C_FLAGS_DEBUG "-Wall -Wextra -O0 -g")
#Unix versions of gcc are a little more picky about having libmath linked explicit
# whereas the osx compiler picks is up automatically. It may be a better
# idea to explicitly link libmath always.
if(UNIX)
    link_libraries(m)
endif(UNIX)


#Path Information
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

add_subdirectory(src)
add_subdirectory(util)

#search for tag programs
find_program(CTAGS ctags)
find_program(ETAGS etags)
find_program(CSCOPE cscope)

file(GLOB_RECURSE MELTMOD_SOURCE_FILES ${CMAKE_SOURCE_DIR} *.[ch])

add_custom_target(ctags
    COMMAND ${CTAGS} ${MELTMOD_SOURCE_FILES}
    WORKING_DIRECTORY ${MeltModel_SOURCE_DIR}/src
    )

add_custom_target(etags
    COMMAND ${ETAGS}  ${MELTMOD_SOURCE_FILES} -f TAGS
    WORKING_DIRECTORY ${MeltModel_SOURCE_DIR}/src
    )

add_custom_target(cscope 
    COMMAND ${CSCOPE} -b ${MELTMOD_SOURCE_FILES} 
    WORKING_DIRECTORY ${MeltModel_SOURCE_DIR}/src
    )

add_custom_target(tags
    COMMAND make ctags
    COMMAND make cscope)

install(FILES input.dat 
	DESTINATION ${CMAKE_SOURCE_DIR}/bin)
