# find src files and header files
file(GLOB srcs *.cpp)
file(GLOB headers *.h)

add_library(libGizmo ${srcs} ${headers})