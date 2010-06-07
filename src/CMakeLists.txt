cmake_minimum_required (VERSION 2.8)
include_directories (${CMAKE_SOURCE_DIR}/src)
set (vramsteg_SRCS vramsteg.cpp
                   Progress.cpp Progress.h)
add_executable (vramsteg ${vramsteg_SRCS})
install (TARGETS vramsteg DESTINATION bin)

