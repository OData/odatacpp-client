set(ODATACPP_CLIENT_SOURCES
  client/odata_client.cpp
  codegen/odata_query_path.cpp
  common/utility.cpp
  common/xmlhelpers.cpp
  communication/http_implement.cpp
  core/odata_context_url_parser.cpp
  core/odata_entity_model_builder.cpp
  core/odata_entity_value.cpp
  core/odata_json_constants.cpp
  core/odata_json_operation_payload_parameter_writer.cpp
  core/odata_json_operation_url_parameter_writer.cpp
  core/odata_json_reader.cpp
  core/odata_json_writer.cpp
  core/odata_primitive_value.cpp
  core/odata_property_map.cpp
  core/odata_structured_value.cpp
  edm/edm_entity_container.cpp
  edm/edm_model.cpp
  edm/edm_model_reader.cpp
  edm/edm_model_utility.cpp
  edm/edm_schema.cpp
  edm/edm_type.cpp
  )

# if(WIN32)
#   add_definitions(
#     -DODATALIB_EXPORTS
#     -D_WINDLL
#     -D_USRDLL
#     )
# endif()

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNINGS} -Werror -pedantic")

add_library(${ODATACPP_CLIENT} ${ODATACPP_CLIENT_SOURCES})

target_link_libraries(${ODATACPP_CLIENT}
  cpprestsdk::cpprest
  ${LIBXML2_LIBRARIES}
  ${EXTRALINKS}
  )

# Portions specific to odatacpp binary versioning.
set (ODATACPP_VERSION_MAJOR 1)
set (ODATACPP_VERSION_MINOR 0)
set (ODATACPP_VERSION_REVISION 0)
if(WIN32)
  set_target_properties(${ODATACPP_CLIENT} PROPERTIES
    OUTPUT_NAME "${ODATACPP_CLIENT}.${ODATACPP_VERSION_MAJOR}.${ODATACPP_VERSION_MINOR}")
else()
  set_target_properties(${ODATACPP_CLIENT} PROPERTIES
    SOVERSION ${ODATACPP_VERSION_MAJOR}.${ODATACPP_VERSION_MINOR})
endif()
