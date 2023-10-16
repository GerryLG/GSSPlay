
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
  gsstrace.cpp
)

target_include_directories (gssxx INTERFACE .)
target_compile_options (gssxx PUBLIC
  ${GSSAPI_CFAGS}
  ${PAC_CFLAGS}
  ${Boost_CFLAGS}
)
target_link_libraries (gssxx PUBLIC
  ${Boost_LIBRARIES}
  ${GSSAPI_LIBRARIES}
  ${PAC_LIBRARIES}
)
