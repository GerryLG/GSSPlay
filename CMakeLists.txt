cmake_minimum_required (VERSION 3.1)
project (KrbPlay)

set (CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_CXX_STANDARD 11)

find_package (Boost REQUIRED COMPONENTS system thread)
find_package (PkgConfig REQUIRED)

pkg_check_modules (GSSAPI REQUIRED krb5-gssapi)

include_directories (${GSSAPI_INCLUDE_DIRS})

add_executable (gsstest gsstest.c)
target_link_libraries (gsstest PUBLIC ${GSSAPI_LIBRARIES})

add_subdirectory (LibGSSXX)
include_directories (LibGSSXX)

add_subdirectory (DaytimeClient)
add_subdirectory (DaytimeServer)

include_directories (${Boost_INCLUDE_DIRS})
link_libraries (Boost::system Boost::thread)

add_executable (testlibgssxx testlibgssxx.cpp)
target_link_libraries (testlibgssxx libgssxx ${GSSAPI_LIBRARIES})
