cmake_minimum_required (VERSION 2.8)
set(CMAKE_LEGACY_CYGWIN_WIN32 0) # Remove when CMake >= 2.8.4 is required
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

set (HAVE_CMAKE true)

project (vramsteg)
include (CXXSniffer)

set (PROJECT_VERSION "1.1.1")

set (PACKAGE "${PROJECT_NAME}")
set (VERSION "${PROJECT_VERSION}")
set (PACKAGE_BUGREPORT "support@taskwarrior.org")
set (PACKAGE_NAME "${PACKAGE}")
set (PACKAGE_TARNAME "${PACKAGE}")
set (PACKAGE_VERSION "${VERSION}")
set (PACKAGE_STRING "${PACKAGE} ${VERSION}")

if (FREEBSD)
SET (VRAMSTEG_MAN1DIR man/man1 CACHE STRING "Installation directory for man pages, section 1")
else (FREEBSD)
SET (VRAMSTEG_MAN1DIR share/man/man1 CACHE STRING "Installation directory for man pages, section 1")
endif (FREEBSD)
SET (VRAMSTEG_DOCDIR  share/doc/clog CACHE STRING "Installation directory for doc files")
SET (VRAMSTEG_BINDIR  bin            CACHE STRING "Installation directory for the binary")

message ("-- Configuring cmake.h")
configure_file (
  ${CMAKE_SOURCE_DIR}/cmake.h.in
  ${CMAKE_SOURCE_DIR}/cmake.h)

add_subdirectory (src)
add_subdirectory (doc)
add_subdirectory (examples)
if (EXISTS ${CMAKE_SOURCE_DIR}/test)
  add_subdirectory (test EXCLUDE_FROM_ALL)
endif (EXISTS ${CMAKE_SOURCE_DIR}/test)

set (doc_FILES NEWS ChangeLog README INSTALL AUTHORS COPYING)
foreach (doc_FILE ${doc_FILES})
  install (FILES ${doc_FILE}  DESTINATION ${VRAMSTEG_DOCDIR})
endforeach (doc_FILE)

# ---

set (CPACK_SOURCE_GENERATOR "TGZ")
set (CPACK_SOURCE_PACKAGE_FILE_NAME ${PACKAGE_NAME}-${PACKAGE_VERSION})
set (CPACK_SOURCE_IGNORE_FILES  "CMakeCache" "CMakeFiles" "CPackConfig" "CPackSourceConfig"
                                "_CPack_Packages" "cmake_install" "install_manifest"
                                "Makefile$" "test" "demo"
                                "src/vramsteg$"
                                "/\\\\.gitignore" "/\\\\.git/" "swp$")
include (CPack)
