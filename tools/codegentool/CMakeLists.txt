# Target
set(ODATACPP_TOOLS_CODEGENTOOL odata-codegen-tool)

set(ODATACPP_TOOLS_CODEGENTOOL_SOURCES
  odata_codegen_initializer.cpp
  odata_codegen_tools.cpp
  odata_codegen_writer.cpp
  )

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNINGS} -Werror -pedantic")

add_executable(${ODATACPP_TOOLS_CODEGENTOOL} ${ODATACPP_TOOLS_CODEGENTOOL_SOURCES})

target_link_libraries(${ODATACPP_TOOLS_CODEGENTOOL}
  ${ODATACPP_CLIENT}
  )
