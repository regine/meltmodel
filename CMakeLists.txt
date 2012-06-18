cmake_minimum_required (VERSION 2.8)

project (MeltModel)

if(APPLE)
    set(CMAKE_C_FLAGS "-m32")
    set(CMAKE_C_FLAGS_DEBUG "-m32 -Wall -O0 -g")
else(APPLE)
    set(CMAKE_C_FLAGS_DEBUG "-Wall -O0 -g")
endif(APPLE)

add_library(closeall closeall.c)
add_library(discharg discharg.c)
add_library(disopt disopt.c)
add_library(globcor globcor.c)
add_library(initial initial.c)
add_library(input input.c)
add_library(radiat radiat.c)
add_library(scaling scaling.c)
add_library(snowinput snowinput.c)
add_library(snowmodel snowmodel.c)
add_library(tindex tindex.c)
add_library(turbul turbul.c)
add_library(userfile userfile.c)
add_library(writeout writeout.c)

add_executable(degree degree.c)
target_link_libraries(degree closeall
                             discharg
                             disopt
                             globcor
                             initial
                             input
                             radiat
                             snowinput
                             snowmodel
                             tindex
                             turbul
                             userfile
                             writeout
                             scaling)

add_executable(meltmod meltmod.c)
target_link_libraries(meltmod closeall
                              discharg
                              disopt
                              globcor
                              initial
                              input
                              radiat
                              snowinput
                              snowmodel
                              turbul
                              userfile
                              writeout)
