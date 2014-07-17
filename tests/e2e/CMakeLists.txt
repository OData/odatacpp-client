include_directories(
  ${UnitTestpp_INCLUDE_DIR}
  ${Utilities_INCLUDE_DIR}
  )

# Target
set(ODATACPP_TESTS_E2E odata-tests-e2e)

set(ODATACPP_TESTS_E2E_SOURCES
  common_tests.cpp
  containment_tests.cpp
  containment_tests.cpp
  function_action_tests.cpp
  derived_type_tests.cpp
  navigation_property_tests.cpp
  e2e_test_case.cpp
  odata_wcf_service.cpp
  edit_link_tests.cpp
  query_option_tests.cpp
  enum_tests.cpp
  reference_tests.cpp
  exchange_service.cpp
  singleton_tests.cpp
  )

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

add_library(${ODATACPP_TESTS_E2E} ${ODATACPP_TESTS_E2E_SOURCES})

target_link_libraries(${ODATACPP_TESTS_E2E}
  ${ODATACPP_LIBRARY}
  ${ODATACPP_CLIENT}
  unittestpp
  utilities
  )
