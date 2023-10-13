
add_library (gssxx
  gssstatus.cpp
  gssname.cpp
  gssbuffer.cpp
  gsscontext.cpp
  gssclientcontext.cpp
  gssservercontext.cpp
  gssservercred.cpp
  gsslocalbuffer.cpp
  gsspac.cpp
)

target_include_directories (gssxx INTERFACE .)
target_include_directories (gssxx PUBLIC ${GSSAPI_INCLUDE_DIRS})
target_include_directories (gssxx PUBLIC ${Boost_INCLUDE_DIRS})
target_include_directories (gssxx PUBLIC ${PAC_INCLUDE_DIRS})

target_compile_options (gssxx PUBLIC ${GSSAPI_CFLAGS_OTHER})
target_compile_options (gssxx PUBLIC ${PAC_CFLAGS_OTHER})

target_link_libraries (gssxx PUBLIC ${GSSAPI_LIBRARIES} ${PAC_LIBRARIES})
