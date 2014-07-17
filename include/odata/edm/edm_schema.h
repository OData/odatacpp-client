#pragma once

#include "odata/common/utility.h"
#include "odata/edm/edm_entity_container.h"

namespace odata { namespace edm
{

class edm_model_utility;

/// <summary>
/// 
/// </summary>
class edm_schema
{
public:
    /// <summary>
    /// 
    /// </summary>
    edm_schema()
    {
    }

    /// <summary>
    /// 
    /// </summary>
    edm_schema(::utility::string_t namesp, ::utility::string_t alias) :
        m_namespace(namesp), m_alias(alias)
    {
    }

#ifdef _MS_WINDOWS_DEBUG
	~edm_schema()
    {
		std::wcout << U("destroy edm_schema") << std::endl;
    }
#endif

    /// <summary>
    /// Adds an entity type to the schema
    /// </summary>
    void add_entity_type(std::shared_ptr<edm_entity_type> et)
    {
        m_entity_types[et->get_name()] = et;
    }

    /// <summary>
    /// Adds an complex type to the schema
    /// </summary>
    void add_complex_type(std::shared_ptr<edm_complex_type> et)
    {
        m_complex_types[et->get_name()] = et;
    }

	/// <summary>
    /// 
    /// </summary>
    void add_enum_type(std::shared_ptr<edm_enum_type> et)
    {
        m_enum_types[et->get_name()] = et;
    }

	/// <summary>
    /// 
    /// </summary>
	void add_operation_type(std::shared_ptr<edm_operation_type> et)
    {
        m_operation_types[et->get_name()] = et;
    }	

    /// <summary>
    /// 
    /// </summary>
    void add_container(std::shared_ptr<edm_entity_container> et)
    {
        m_entity_containers[et->get_name()] = et;
    }

    /// <summary>
    /// Looks up an entity type of the schema by name.
    /// </summary>
    /// <param name="name">The qualified or unqualified name of the entity type.</param>
    /// <returns>A pointer to the type if found, an empty pointer otherwise.</returns>
    ODATACPP_API std::shared_ptr<edm_entity_type> find_entity_type(::utility::string_t name) const;

    /// <summary>
    /// Looks up a complex type of the schema by name.
    /// </summary>
    /// <param name="name">The qualified or unqualified name of the complex type.</param>
    /// <returns>A pointer to the type if found, an empty pointer otherwise.</returns>
    ODATACPP_API std::shared_ptr<edm_complex_type> find_complex_type(::utility::string_t name) const;

	/// <summary>
    /// Looks up an enum type of the schema by name.
    /// </summary>
    /// <param name="name">The qualified or unqualified name of the enum type.</param>
    /// <returns>A pointer to the type if found, an empty pointer otherwise.</returns>
	ODATACPP_API std::shared_ptr<edm_enum_type> find_enum_type(::utility::string_t name) const;

	/// <summary>
    /// Looks up an operation type of the schema by name.
    /// </summary>
    /// <param name="name">The qualified or unqualified name of the entity type.</param>
    /// <returns>A pointer to the type if found, an empty pointer otherwise.</returns>
	ODATACPP_API std::shared_ptr<edm_operation_type> find_operation_type(::utility::string_t name) const;

    /// <summary>
    /// Looks up an entity container of the schema by name.
    /// </summary>
    /// <param name="name">The qualified or unqualified name of the entity container; an empty string refers to the default container</param>
    /// <returns>A pointer to the container if found, an empty pointer otherwise.</returns>
    ODATACPP_API std::shared_ptr<edm_entity_container> find_container(::utility::string_t name = U("")) const;

    const std::unordered_map<::utility::string_t, std::shared_ptr<edm_entity_type>>& get_entity_types() const
    {
        return m_entity_types;
    }

    const std::unordered_map<::utility::string_t, std::shared_ptr<edm_complex_type>>& get_complex_types() const
    {
        return m_complex_types;
    }

	const std::unordered_map<::utility::string_t, std::shared_ptr<edm_enum_type>>& get_enum_types() const
    {
        return m_enum_types;
    }

    const std::unordered_map<::utility::string_t, std::shared_ptr<edm_operation_type>>& get_operation_types() const
    {
        return m_operation_types;
    }

    const std::unordered_map<::utility::string_t, std::shared_ptr<edm_entity_container>>& get_containers() const
    {
        return m_entity_containers;
    }

    const ::utility::string_t& get_name() const
    {
        return m_namespace;
    }

private:
	friend class edm_model_utility;

    ::utility::string_t m_namespace;
    ::utility::string_t m_alias;
	std::unordered_map<::utility::string_t, std::shared_ptr<edm_entity_type>> m_entity_types;
	std::unordered_map<::utility::string_t, std::shared_ptr<edm_complex_type>> m_complex_types;
	std::unordered_map<::utility::string_t, std::shared_ptr<edm_enum_type>> m_enum_types;
	std::unordered_map<::utility::string_t, std::shared_ptr<edm_operation_type>> m_operation_types;
	std::unordered_map<::utility::string_t, std::shared_ptr<edm_entity_container>> m_entity_containers;

	std::vector<std::shared_ptr<edm_collection_type>> m_collection_navigation_types;
};

}}