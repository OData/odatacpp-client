include_directories(include)

if(UNIX)
  set(CXX_FLAGS "${CXX_FLAGS} -include ${ODATACPP_INCLUDE_DIR}/compat/linux_compat.h")
elseif(WIN32)
  add_definitions(-DCOMMONUTILITIES_EXPORTS)
endif()

add_library(${LIB}utilities
  os_utilities.cpp
  )
  
target_link_libraries(${LIB}utilities
  ${LIB}unittestpp
  #${BOOST_SYSTEM_LIBRARY}
  #${BOOST_THREAD_LIBRARY}
  )
