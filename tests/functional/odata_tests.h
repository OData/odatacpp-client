#pragma once

#include "odata/client/odata_client.h"

#include "unittestpp.h"
#include "os_utilities.h"


extern const char* test_model_string;
extern std::shared_ptr<::odata::edm::edm_model> g_test_model;
extern ::utility::string_t g_service_root_url;

std::shared_ptr<::odata::edm::edm_model> get_test_model();