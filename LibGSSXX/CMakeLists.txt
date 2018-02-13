include_directories (${Boost_INCLUDE_DIRS})

add_library (libgssxx
  gssbuffer.cpp
  gsscontext.cpp
  gssclientcontext.cpp
  gssname.cpp
  gssservercontext.cpp
  gssservercred.cpp
  gssstatus.cpp
  gssvectorbuffer.cpp
  gssauthdata.cpp
  )
target_include_directories(libgssxx PUBLIC ${GSSAPI_INCLUDE_DIRS})
target_compile_options(libgssxx PUBLIC ${GSSAPI_CFLAGS_OTHER})
