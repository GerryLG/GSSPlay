add_executable (daytimeserver
  daytimeserver.cpp
  tcp_server.cpp
  tcp_connection.cpp
)
target_link_libraries (daytimeserver
  gssxx
)
