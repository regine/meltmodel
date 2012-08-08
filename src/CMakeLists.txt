# src/CMakeLists.txt

# Add each sublibrary of melt_mod as a static library
# This is not technically what they are, since Meltmod 
# works in a mutable global state, and most of the
# functions in these files rely on, and manipulate that
# global state, they really mean little in a vacuum.
#
# That being said, it doesn't make a lot of sense to
# have to recompile everything every time, they've been
# broken up as much as possible, and it's left to the
# linker to make sure the symbols get resolved in the end.
# 
# The caveat here being that the circular dependencies
# between many of these files can confuse the linker,
# OS X 10.6 doesn't seem to have a problem, but Ubuntu 12.4
# and later sure can.
#
# Linker errors can be fixed as below, by explicitly
# linking these guys together, as with snowmodel and turbul.
# In the future, this should probably get done for all
# of these guys.
add_library(closeall closeall.c)
add_library(discharg discharg.c)
add_library(disopt disopt.c)
add_library(globcor globcor.c)
add_library(initial initial.c)
add_library(input input.c)
add_library(radiat radiat.c)
add_library(scaling scaling.c)
add_library(snowmodel snowinput.c snowmodel.c)
add_library(tindex tindex.c)
add_library(turbul turbul.c)
target_link_libraries(turbul snowmodel)
add_library(userfile userfile.c)
add_library(writeout writeout.c)

# grid.c is an exception, it does not depend
# on anything but the c std library.
add_library(grid grid.c)

# Create the target for the temprature-index model, and 
# link together everything it needs
add_executable(degree degree.c)
target_link_libraries(degree closeall
                             discharg
                             disopt
                             globcor
                             grid
                             initial
                             input
                             radiat
                             snowmodel
                             tindex
                             turbul
                             userfile
                             writeout
                             scaling)

# Create the target for the energy-balance model, and
# link it to it's required libraries
add_executable(meltmod meltmod.c)
target_link_libraries(meltmod closeall
                              discharg
                              disopt
                              globcor
                              grid
                              initial
                              input
                              radiat
                              snowmodel
                              turbul
                              userfile
                              writeout)

install(TARGETS degree meltmod
    RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin)
