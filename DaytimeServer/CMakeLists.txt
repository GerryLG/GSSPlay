include_directories (${Boost_INCLUDE_DIRS})
link_libraries (Boost::system Boost::thread ${GSSAPI_LIBRARIES} libgssxx)

add_executable (daytimeserver daytimeserver.cpp tcp_server.cpp tcp_connection.cpp)
