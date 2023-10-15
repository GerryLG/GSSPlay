cmake_minimum_required (VERSION 3.15)
project (GSSPlay)

set (CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_CXX_STANDARD 17)

set (GSSAPI_PKG_NAME "" CACHE STRING "The name of the GSSAPI library package that PkgConfig will search for.")

if ("$CACHE{GSSAPI_PKG_NAME}" STREQUAL "")
  message (FATAL_ERROR "Must specify GSSAPI_PKG_NAME")
endif ("$CACHE{GSSAPI_PKG_NAME}" STREQUAL "")

find_package (Boost REQUIRED COMPONENTS system thread)
find_package (PkgConfig REQUIRED)

pkg_check_modules (GSSAPI REQUIRED ${GSSAPI_PKG_NAME})
pkg_check_modules (PAC REQUIRED talloc ndr_krb5pac)

add_subdirectory (GSSXX)

add_subdirectory (DaytimeClient)
add_subdirectory (DaytimeServer)


add_executable (gsstest gsstest.c)
target_link_libraries (gsstest PUBLIC ${GSSAPI_LIBRARIES})
target_compile_options (gsstest PUBLIC ${GSSAPI_CFLAGS})

add_executable (testlibgssxx testlibgssxx.cpp)
target_link_libraries (testlibgssxx gssxx)
