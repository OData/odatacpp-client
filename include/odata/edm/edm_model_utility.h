#pragma once

#include "odata/edm/odata_edm.h"

namespace odata { namespace edm
{

class edm_model_utility
{
public:
	ODATACPP_API static std::shared_ptr<edm_named_type> get_edm_type_from_name(const ::utility::string_t& type_name);
	ODATACPP_API static ::utility::string_t get_strong_type_name_from_edm_type_name(const std::shared_ptr<edm_primitive_type>& primitive_type);
	ODATACPP_API static ::utility::string_t get_strong_type_default_value_from_edm_type_name(const std::shared_ptr<edm_primitive_type>& primitive_type);
	ODATACPP_API static std::shared_ptr<edm_named_type> get_edm_primitive_type_from_name(const ::utility::string_t& type_name);
    ODATACPP_API static ::utility::string_t get_type_kind_name_from_edm_type(const std::shared_ptr<edm_named_type>& ptype);
    ODATACPP_API static bool get_primitive_kind_from_edm_type(const std::shared_ptr<edm_named_type>& edm_type, edm_primitive_type_kind_t& primitive_kind);
	ODATACPP_API static void resolve_edm_types_after_parsing(const std::shared_ptr<edm_model>& model);
	ODATACPP_API static std::shared_ptr<::odata::edm::edm_named_type> get_property_type_from_name(const std::shared_ptr<::odata::edm::edm_entity_type>& entity_type, const ::utility::string_t& property_name);

	ODATACPP_API static std::shared_ptr<edm_named_type> get_collection_element_type(const std::shared_ptr<edm_named_type>& input_type);
	ODATACPP_API static std::shared_ptr<edm_named_type> get_navigation_element_type(const std::shared_ptr<edm_named_type>& input_type);

private:
	static std::shared_ptr<edm_named_type> resolve_type_from_name(const std::shared_ptr<edm_model>& model, ::utility::string_t qualified_name);
	static void resolve_type_under_structured_type(const std::shared_ptr<edm_model>& model, const std::shared_ptr<edm_structured_type>& structyred_type);
	static void resolve_type_under_operation_type(const std::shared_ptr<edm_model>& model, const std::shared_ptr<edm_operation_type>& operationType);
	static std::shared_ptr<edm_named_type> resolve_undetermined_type(const std::shared_ptr<edm_model>& model, const std::shared_ptr<edm_named_type>& undeterminedType);
	static void resolve_type_under_entity_container(const std::shared_ptr<edm_model>& model, const std::shared_ptr<edm_entity_container>& entity_container);
	static void resovle_entity_base_type(const std::shared_ptr<edm_model>& model, const std::shared_ptr<edm_entity_type>& entity_type);
	static void resovle_complex_base_type(const std::shared_ptr<edm_model>& model, const std::shared_ptr<edm_complex_type>& complex_type);
	static void resolve_navigation_path_for_non_contained_navigation(const std::shared_ptr<edm_model>& model, const std::shared_ptr<edm_entity_container>& entity_container);
	static ::utility::string_t get_collection_element_name(const ::utility::string_t& collection_full_name);
	static std::shared_ptr<edm_navigation_type> get_navigation_property_from_path(const std::shared_ptr<edm_model>& model, const std::shared_ptr<edm_entity_type>& entity_type, const ::utility::string_t& navigation_path);
};

}}