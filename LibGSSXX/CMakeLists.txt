include_directories (${Boost_INCLUDE_DIRS})

add_library (libgssxx
  gssstatus.cpp
  gssname.cpp
  gssbuffer.cpp
  gssauthdata.cpp
  gsscontext.cpp
  gssclientcontext.cpp
  gssservercontext.cpp
  gssservercred.cpp
  gssvectorbuffer.cpp
  gssder.cpp
  )

target_include_directories (libgssxx PUBLIC ${GSSAPI_INCLUDE_DIRS})
target_compile_options (libgssxx PUBLIC ${GSSAPI_CFLAGS_OTHER})
