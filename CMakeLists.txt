set(CMAKE_LEGACY_CYGWIN_WIN32 0)
cmake_minimum_required(VERSION 2.6)
project(odatacpp)

enable_testing()

set(WARNINGS)

# Platform (not compiler) specific settings
if(IOS)
  set(IOS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/ios")
  set(Boost_FRAMEWORK "-F ${IOS_SOURCE_DIR} -framework boost")
  set(Boost_INCLUDE_DIR "${IOS_SOURCE_DIR}/boost.framework/Headers")

  set(LIBXML2_INCLUDE_DIR "/usr/include/libxml2")
  
  # The cxx_flags must be set here, because the ios-cmake toolchain file unfortunately sets "-headerpad_max_install_names" which is not a valid clang flag.
  set(CMAKE_CXX_FLAGS "-fvisibility=hidden -fvisibility-inlines-hidden")
   
  set(BUILD_SHARED_LIBS OFF)
elseif(UNIX) # This includes OSX
  find_package(Boost REQUIRED COMPONENTS locale filesystem system thread chrono)
  find_package(LibXml2 REQUIRED)
  find_package(OpenSSL REQUIRED)
  
  option(BUILD_SHARED_LIBS "Build shared Libraries." ON)
# elseif(WIN32)
#   option(BUILD_SHARED_LIBS "Build shared Libraries." ON)

#   add_definitions(
#     -DWIN32
#     -DUNICODE
#     -D_UNICODE)

#   if(NOT BUILD_SHARED_LIBS)
#     # This causes cmake to not link the test libraries separately, but instead hold onto their object files.
#     set(TEST_LIBRARY_TARGET_TYPE OBJECT)
#   endif()

#   set(LIB lib)
else()
  message(FATAL_ERROR "-- Unsupported Build Platform.")
endif()

# Compiler (not platform) specific settings
if(("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang") OR IOS)
  message("-- Setting clang options")

  set(WARNINGS "-Wall -Wextra -Wcast-qual -Wformat=2 -Winit-self -Winvalid-pch -Wmissing-format-attribute -Wmissing-include-dirs -Wpacked -Wredundant-decls")
  set(OSX_SUPPRESSIONS "-Wno-overloaded-virtual -Wno-sign-conversion -Wno-deprecated -Wno-unknown-pragmas -Wno-reorder -Wno-char-subscripts -Wno-switch -Wno-unused-parameter -Wno-unused-variable -Wno-deprecated -Wno-unused-value -Wno-unknown-warning-option -Wno-return-type-c-linkage -Wno-unused-function -Wno-sign-compare -Wno-shorten-64-to-32 -Wno-reorder -Wno-ignored-qualifiers -Wno-sometimes-uninitialized -Wno-logical-op-parentheses -Wno-unused-private-field")
  set(WARNINGS "${WARNINGS} ${OSX_SUPPRESSIONS}")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -Wno-return-type-c-linkage -Wno-unneeded-internal-declaration")
  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-strict-aliasing")
  set(STRICT_CXX_FLAGS ${WARNINGS} "-Werror -pedantic")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
  message("-- Setting gcc options")

  set(WARNINGS "-Wall -Wextra -Wunused-parameter -Wcast-align -Wcast-qual -Wformat=2 -Winit-self -Winvalid-pch -Wmissing-format-attribute -Wmissing-include-dirs -Wpacked -Wredundant-decls -Wunreachable-code")
  set(LINUX_SUPPRESSIONS "-Wno-deprecated -Wno-unknown-pragmas -Wno-reorder -Wno-unused-function -Wno-char-subscripts -Wno-switch -Wno-unused-but-set-parameter -Wno-unused-value -Wno-unused-local-typedefs -Wno-unused-parameter -Wno-deprecated-declarations -Wno-ignored-qualifiers -Wno-unused-variable -Wno-sign-conversion -Wno-sign-compare -Wno-parentheses")

  set(WARNINGS "${WARNINGS} ${LINUX_SUPPRESSIONS}")
  set(LD_FLAGS "${LD_FLAGS} -Wl,-z,defs")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-strict-aliasing")
  set(STRICT_CXX_FLAGS ${WARNINGS} "-Werror -pedantic")
else()
  message("-- Unknown compiler, success is doubtful.")
endif()

# Reconfigure final output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/output)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/output)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/output)

# These settings can be used by the test targets
set(ODATACPP_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(Casablanca_LIBRARIES /usr/lib/x86_64-linux-gnu/libcpprest.so)
set(Casablanca_INCLUDE_DIR /usr/include/cpprest)
set(ODATACPP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(ODATACPP_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include ${Casablanca_INCLUDE_DIR} ${Boost_INCLUDE_DIR} ${LIBXML2_INCLUDE_DIR})
set(ODATACPP_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# OData library targets
set(ODATACPP_CLIENT ${LIB}odata-client)

# Everything in the project needs access to the odatacpp include directories
include_directories(${ODATACPP_INCLUDE_DIRS})

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(tools)
