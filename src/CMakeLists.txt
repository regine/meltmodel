
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
                              snowmodel
                              turbul
                              userfile
                              writeout)
