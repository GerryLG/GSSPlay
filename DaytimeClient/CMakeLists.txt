include_directories (${Boost_INCLUDE_DIRS})
link_libraries (Boost::system Boost::thread ${GSSAPI_LIBRARIES} libgssxx)

add_executable (daytimeclient daytimeclient.cpp)

