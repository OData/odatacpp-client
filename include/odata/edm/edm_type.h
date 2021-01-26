//---------------------------------------------------------------------
// <copyright file="edm_type.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#include "odata/common/utility.h"

namespace odata { namespace edm
{

#define undefined_value 0xffffffff

class edm_schema;
class edm_model_reader;

/// <summary>
/// Defines EDM metatypes.
/// </summary>
enum edm_type_kind_t
{
	None,
	Primitive,
	Entity,
	Complex,
	Collection,
	Enum,
	Link,
	Navigation,
	Operation,
	PayloadAnnotation,
	Unknown,
};

/// <summary>
/// Enumerates the kinds of Edm Primitives.
/// </summary>
enum edm_primitive_type_kind_t
{
	NoneVal,
	Binary,
	Boolean,
	Byte,
	DateTimeOffset,
	Duration,
	Decimal,
	Double,
	Guid,
	Int16,
	Int32,
	Int64,
	SByte,
	Single,
	String,
	Stream,
};

class edm_named_type
{
public:
	edm_named_type() :  m_name(), m_namespace(), m_type_kind(edm_type_kind_t::None)
	{
	}

	edm_named_type(edm_type_kind_t type_kind) :  m_name(), m_namespace(), m_type_kind(type_kind)
	{
	}

	edm_named_type(::odata::string_t name, ::odata::string_t name_space, edm_type_kind_t type_kind)
		: m_name(std::move(name)), m_namespace(std::move(name_space)), m_type_kind(type_kind)
	{
	}

	virtual ~edm_named_type() = default;

	const ::odata::string_t& get_name() const
	{
		return m_name;
	}

	::odata::string_t get_full_name() const
	{
		if (m_namespace.empty())
		{
			return m_name;
		}

		return m_namespace + _XPLATSTR(".") + m_name;
	}

	void set_name(::odata::string_t name)
	{
		m_name = std::move(name);
	}

	const ::odata::string_t& get_namespace() const
	{
		return m_namespace;
	}

	void set_namespace(::odata::string_t name_space)
	{
		m_namespace = std::move(name_space);
	}

	const edm_type_kind_t& get_type_kind() const
	{
		return m_type_kind;
	}

	void set_type_kind(edm_type_kind_t kind)
	{
		m_type_kind = kind;
	}

	ODATACPP_CLIENT_API static std::shared_ptr<edm_named_type> EDM_UNKNOWN();

protected:
	static std::mutex &mutex();

protected:
	friend class edm_schema;

	::odata::string_t m_name;
	::odata::string_t m_namespace;
	edm_type_kind_t m_type_kind;

	static std::shared_ptr<edm_named_type> _EDM_UNKNOWN;
};

class edm_primitive_type : public edm_named_type
{
public:
	const edm_primitive_type_kind_t get_primitive_kind() const
	{
		return m_primitive_kind;
	}

	bool type_equal(std::shared_ptr<edm_named_type> type) const
	{
		if (type && type->get_type_kind() == edm_type_kind_t::Primitive)
		{
			edm_primitive_type *p_primitive_type = (edm_primitive_type*)type.get();
			if (p_primitive_type && p_primitive_type->m_primitive_kind == m_primitive_kind)
			{
				return true;
			}
		}

		return false;
	}

	ODATACPP_CLIENT_API static std::shared_ptr<edm_primitive_type> BINARY();
	ODATACPP_CLIENT_API static std::shared_ptr<edm_primitive_type> BOOLEAN();
	ODATACPP_CLIENT_API static std::shared_ptr<edm_primitive_type> BYTE();
	ODATACPP_CLIENT_API static std::shared_ptr<edm_primitive_type> DATETIMEOFFSET();
	ODATACPP_CLIENT_API static std::shared_ptr<edm_primitive_type> DURATION();
	ODATACPP_CLIENT_API static std::shared_ptr<edm_primitive_type> DECIMAL();
	ODATACPP_CLIENT_API static std::shared_ptr<edm_primitive_type> DOUBLE();
	ODATACPP_CLIENT_API static std::shared_ptr<edm_primitive_type> GUID();
	ODATACPP_CLIENT_API static std::shared_ptr<edm_primitive_type> INT16();
	ODATACPP_CLIENT_API static std::shared_ptr<edm_primitive_type> INT32();
	ODATACPP_CLIENT_API static std::shared_ptr<edm_primitive_type> INT64();
	ODATACPP_CLIENT_API static std::shared_ptr<edm_primitive_type> SBYTE();
	ODATACPP_CLIENT_API static std::shared_ptr<edm_primitive_type> SINGLE();
	ODATACPP_CLIENT_API static std::shared_ptr<edm_primitive_type> STRING();
	ODATACPP_CLIENT_API static std::shared_ptr<edm_primitive_type> STREAM();
	ODATACPP_CLIENT_API static std::shared_ptr<edm_primitive_type> UNKNOWN();

private:
	edm_primitive_type(::odata::string_t name, edm_primitive_type_kind_t primitive_kind)
		: edm_named_type(name, _XPLATSTR(""), edm_type_kind_t::Primitive), m_primitive_kind(primitive_kind)
	{
	}

	static std::mutex &mutex();

	edm_primitive_type_kind_t m_primitive_kind;

	static std::shared_ptr<edm_primitive_type> _BINARY;
	static std::shared_ptr<edm_primitive_type> _BOOLEAN;
	static std::shared_ptr<edm_primitive_type> _BYTE;
	static std::shared_ptr<edm_primitive_type> _DATETIMEOFFSET;
	static std::shared_ptr<edm_primitive_type> _DURATION;
	static std::shared_ptr<edm_primitive_type> _DECIMAL;
	static std::shared_ptr<edm_primitive_type> _DOUBLE;
	static std::shared_ptr<edm_primitive_type> _GUID;
	static std::shared_ptr<edm_primitive_type> _INT16;
	static std::shared_ptr<edm_primitive_type> _INT32;
	static std::shared_ptr<edm_primitive_type> _INT64;
	static std::shared_ptr<edm_primitive_type> _SBYTE;
	static std::shared_ptr<edm_primitive_type> _SINGLE;
	static std::shared_ptr<edm_primitive_type> _STRING;
	static std::shared_ptr<edm_primitive_type> _STREAM;
	static std::shared_ptr<edm_primitive_type> _UNKNOWN;

};

/// <summary>
/// Represents a CSDL Property, used in ComplexType and EntityType definitions.
/// </summary>
class edm_property_type
{
public:
	/// <summary>
	/// Constructor
	/// </summary>
	edm_property_type()
	:	m_is_nullable(true), m_is_unicode(false), m_name(), m_default(), m_maxLength(undefined_value), m_scale(0), m_precision(undefined_value), m_type()
	{
	}

	edm_property_type(const ::odata::string_t& name)
	:	m_is_nullable(true), m_is_unicode(false), m_name(name), m_default(), m_maxLength(undefined_value), m_scale(0), m_precision(undefined_value), m_type()
	{
	}

	edm_property_type(const ::odata::string_t& name, bool is_nullable, unsigned int max_length, bool is_unicode, unsigned int scale) :
		m_is_nullable(is_nullable), m_is_unicode(is_unicode), m_name(name), m_default(), m_maxLength(max_length), m_scale(scale), m_precision(undefined_value), m_type()
	{
	}

	edm_property_type(const ::odata::string_t& name, bool is_nullable, std::shared_ptr<edm_named_type> type) :
		m_is_nullable(is_nullable), m_is_unicode(false), m_name(name), m_default(), m_maxLength(undefined_value), m_scale(0), m_precision(undefined_value), m_type(type)
	{
	}

	/// <summary>
	/// Gets the (unqualified) name of the property.
	/// </summary>
	/// <returns>The name of the property.</returns>
	const ::odata::string_t& get_name() const
	{
		return m_name;
	}

	/// <summary>
	/// Gets the nullable property of the property
	/// </summary>
	/// <returns>true if the property can be absent in a entity, false otherwise</returns>
	bool is_nullable() const
	{
		return m_is_nullable;
	}

	std::shared_ptr<edm_named_type> get_property_type()
	{
		return m_type;
	}

	std::shared_ptr<edm_named_type const> const& get_property_type() const
	{
		return m_type;
	}

	void set_property_type(std::shared_ptr<edm_named_type> type)
	{
		m_type = type;
	}

	void set_precision(unsigned int precision)
	{
		m_precision = precision;
	}

	/// <summary>
	/// Gets the default value of the property, as represented in the CSDL.
	/// </summary>
	/// <returns>A string representing the default value of the property.</returns>
	const ::odata::string_t& default_value() const
	{
		return m_default;
	}

private:
	friend class edm_structured_type;

	bool m_is_nullable;
	bool m_is_unicode;
	::odata::string_t m_name;
	::odata::string_t m_default;
	unsigned int m_maxLength;
	unsigned int m_scale;
	unsigned int m_precision;

	std::shared_ptr<edm_named_type> m_type;
};

class edm_collection_type : public edm_named_type
{
public:
	edm_collection_type(::odata::string_t name)
		: edm_named_type(name, _XPLATSTR(""), edm_type_kind_t::Collection)
	{
	}

	edm_collection_type(::odata::string_t name, std::shared_ptr<edm_named_type> element_type)
		: edm_named_type(name, _XPLATSTR(""), edm_type_kind_t::Collection), m_element_type(element_type)
	{
	}

	std::shared_ptr<edm_named_type> get_element_type() const
	{
		return m_element_type.lock();
	}

	void set_element_type(std::shared_ptr<edm_named_type> type)
	{
		m_element_type = type;
	}

protected:
	std::weak_ptr<edm_named_type> m_element_type;
};

class edm_payload_annotation_type : public edm_named_type
{
public:
	edm_payload_annotation_type(::odata::string_t name)
		: edm_named_type(name, _XPLATSTR(""), edm_type_kind_t::PayloadAnnotation)
	{
	}
};

/// <summary>
/// Represents what is shared between edm_complex_type and edm_entity_type.
/// </summary>
class edm_structured_type : public edm_named_type
{
public:
	/// <summary>
	/// Constructor
	/// </summary>
	/// <param name="name">The name of the type.</param>
	edm_structured_type(::odata::string_t name, ::odata::string_t name_space, edm_type_kind_t type_kind, ::odata::string_t basetype, bool isAbstract, bool isOpenType)
		: edm_named_type(name, name_space, type_kind), m_baseTypeName(basetype), m_base_type(nullptr), m_is_abstract(isAbstract), m_is_openType(isOpenType), m_properties()
	{
	}

	/// <summary>
	/// Adds a property to the type.
	/// </summary>
	/// <param name="prop">A pointer to the property to add.</param>
	void add_property(std::shared_ptr<edm_property_type> prop)
	{
		m_properties[prop->get_name()] = prop;
	}

	/// <summary>
	/// Gets the beginning iterator of the properties of the type
	/// </summary>
	std::unordered_map<::odata::string_t, std::shared_ptr<edm_property_type>>::const_iterator begin() const
	{
		return m_properties.cbegin();
	}

	/// <summary>
	/// Gets the end iterator of the properties of the type
	/// </summary>
	std::unordered_map<::odata::string_t, std::shared_ptr<edm_property_type>>::const_iterator end() const
	{
		return m_properties.cend();
	}

	std::vector<std::shared_ptr<edm_property_type>> get_properties_vector() const
	{
		std::vector<std::shared_ptr<edm_property_type>> ret;
		ret.reserve(m_properties.size());
		for (auto it : m_properties)
		{
			ret.emplace_back(it.second);
		}
		return ret;
	}

	const ::odata::string_t get_base_type_name() const
	{
		return m_baseTypeName;
	}

	void set_base_type(const std::shared_ptr<edm_structured_type>& base_entity_type)
	{
		m_base_type = base_entity_type;
	}

	const std::shared_ptr<edm_structured_type>& get_base_type()
	{
		return m_base_type;
	}

	bool is_abstract() const
	{
		return m_is_abstract;
	}

	bool is_open() const
	{
		return m_is_openType;
	}

	/// <summary>
	/// Looks up a property of the type by name.
	/// </summary>
	/// <param name="name">The name of the property.</param>
	/// <returns>A pointer to the property if found, an empty pointer otherwise.</returns>
	ODATACPP_CLIENT_API std::shared_ptr<edm_property_type> find_property(::odata::string_t name) const;

protected:
	friend class edm_schema;

	::odata::string_t m_baseTypeName;
	std::shared_ptr<edm_structured_type> m_base_type;

	bool m_is_abstract;
	bool m_is_openType;

	std::unordered_map<::odata::string_t, std::shared_ptr<edm_property_type>> m_properties;
};

class edm_enum_member
{
public:
	/// <summary>
	/// Constructor
	/// </summary>
	edm_enum_member() : m_name(_XPLATSTR("")), m_value(0)
	{
	}

	edm_enum_member(::odata::string_t name, unsigned long value) : m_name(name), m_value(value)
	{
	}

	const ::odata::string_t& get_enum_member_name() const
	{
		return m_name;
	}

	void set_enum_member_name(const ::odata::string_t& name)
	{
		m_name = name;
	}

	unsigned long get_enum_member_value() const
	{
		return m_value;
	}

	void set_enum_member_value(unsigned long value)
	{
		m_value = value;
	}

private:
	friend class edm_enum_type;

	::odata::string_t m_name;
	unsigned long m_value;
};

/// <summary>
///
/// </summary>
class edm_enum_type : public edm_named_type
{
public:
	/// <summary>
	/// Constructor
	/// </summary>
	edm_enum_type(::odata::string_t name, ::odata::string_t name_space, ::odata::string_t underlying_type, bool is_flag)
		: edm_named_type(name, name_space, edm_type_kind_t::Enum), m_underlying_type(underlying_type), m_is_flag(is_flag)
	{
	}

	/// <summary>
	///
	/// </summary>
	void add_enum_member(std::shared_ptr<edm_enum_member> member)
	{
		m_members.emplace_back(member);
	}

	const std::vector<std::shared_ptr<edm_enum_member>>& get_enum_members()
	{
		return m_members;
	}

	bool is_flag()
	{
		return m_is_flag;
	}

private:
	friend class edm_schema;

	::odata::string_t m_underlying_type;
	bool m_is_flag;

	std::vector<std::shared_ptr<edm_enum_member>> m_members;
};

/// <summary>
///
/// </summary>
class edm_complex_type : public edm_structured_type
{
public:
	/// <summary>
	/// Constructor
	/// </summary>
	/// <param name="name">The name of the type.</param>
	edm_complex_type(::odata::string_t name, ::odata::string_t name_space, ::odata::string_t basetype = _XPLATSTR(""), bool isAbstract = false, bool isOpenType = false)
		: edm_structured_type(name, name_space, edm_type_kind_t::Complex, basetype, isAbstract, isOpenType)
	{
	}

private:
	friend class edm_schema;
};

/// <summary>
///
/// </summary>
class edm_navigation_source;

class edm_navigation_type : public edm_named_type
{
public:
	edm_navigation_type(::odata::string_t naviagtion_type, ::odata::string_t partner_name, bool is_contained = false)
		: edm_named_type(naviagtion_type, _XPLATSTR(""), edm_type_kind_t::Navigation), m_partner_name(partner_name), m_is_contained(is_contained)
	{
		m_naviagtion_type = ::odata::make_shared<edm_named_type>(naviagtion_type, _XPLATSTR(""), edm_type_kind_t::Unknown);
	}

	std::shared_ptr<edm_named_type> get_navigation_type() const
	{
		return m_naviagtion_type.lock();
	}

	void set_navigation_type(const std::shared_ptr<edm_named_type>& naviagtion_type)
	{
		m_naviagtion_type = naviagtion_type;
	}

	std::shared_ptr<edm_navigation_source> get_binded_navigation_source() const
	{
		return m_is_contained ? nullptr : m_binded_navigation_source.lock();
	}

	void set_binded_navigation_source(const std::shared_ptr<edm_navigation_source>& entity_set)
	{
		if (!m_is_contained)
		{
			m_binded_navigation_source = entity_set;
		}
	}

	bool is_contained() const
	{
		return m_is_contained;
	}

private:
	friend class edm_schema;

	std::weak_ptr<edm_named_type> m_naviagtion_type;
	::odata::string_t m_partner_name;
	bool m_is_contained;
	// only works for navigation that has m_is_contained = false
	std::weak_ptr<edm_navigation_source> m_binded_navigation_source;
};

/// <summary>
///
/// </summary>
enum EdmOperationKind
{
	Action = 0,
	Function
};

/// <summary>
///
/// </summary>
class edm_operation_parameter
{
public:
	edm_operation_parameter() :
		m_param_name(_XPLATSTR("")), m_param_type(nullptr), m_is_nullable(false)
	{
	}

	edm_operation_parameter(::odata::string_t param_name, std::shared_ptr<edm_named_type> param_type) :
		m_param_name(param_name), m_param_type(param_type), m_is_nullable(false)
	{
	}

	const std::shared_ptr<edm_named_type>& get_param_type() const
	{
		return m_param_type;
	}

	void set_param_type(std::shared_ptr<edm_named_type> type)
	{
		m_param_type = type;
	}

	const ::odata::string_t& get_param_name() const
	{
		return m_param_name;
	}

private:
	friend class edm_operation_type;

	::odata::string_t m_param_name;
	std::shared_ptr<edm_named_type> m_param_type;
	bool m_is_nullable;
};

/// <summary>
///
/// </summary>
class edm_operation_type : public edm_named_type
{
public:
	edm_operation_type(::odata::string_t name, ::odata::string_t name_space, bool is_bound, ::odata::string_t path, EdmOperationKind operation_kind, bool is_composable)
	:	edm_named_type(name, name_space, edm_type_kind_t::Operation), m_is_bound(is_bound), m_path(path), m_parameters(), m_return_type(), m_operation_kind(operation_kind), m_is_composable(is_composable)
	{
		if (operation_kind == EdmOperationKind::Action)
		{
			m_is_composable = false;
		}
	}

	void add_operation_parameter(std::shared_ptr<edm_operation_parameter> parameter)
	{
		m_parameters.emplace_back(parameter);
	}

	void set_return_type(std::shared_ptr<edm_named_type> return_type)
	{
		m_return_type = return_type;
	}

	const std::vector<std::shared_ptr<edm_operation_parameter>>& get_operation_parameters()
	{
		return m_parameters;
	}

	void set_return_type_name(const ::odata::string_t& return_type_name)
	{
		m_return_type_name = return_type_name;
	}

	const ::odata::string_t& get_return_type_name() const
	{
		return m_return_type_name;
	}

	std::shared_ptr<edm_named_type> get_operation_return_type()
	{
		return m_return_type;
	}

	bool is_bound() const
	{
		return m_is_bound;
	}

	bool is_function() const
	{
		return m_operation_kind == EdmOperationKind::Function;
	}

private:
	friend class edm_schema;
	friend class edm_model;

	bool m_is_bound;
	::odata::string_t m_path;
	std::vector<std::shared_ptr<edm_operation_parameter>> m_parameters;
	std::shared_ptr<edm_named_type> m_return_type;
	::odata::string_t m_return_type_name;
	EdmOperationKind m_operation_kind;
	bool m_is_composable;
};


/// <summary>
/// Represents the type of an entity in a container
/// </summary>
class edm_entity_type : public edm_structured_type
{
public:
	/// <summary>
	/// Constructor
	/// </summary>
	/// <param name="name">The name of the type.</param>
	edm_entity_type(::odata::string_t name, ::odata::string_t name_space, ::odata::string_t basetype = _XPLATSTR(""), bool isAbstract = false, bool isOpenType = false, bool hasStream = false)
		: edm_structured_type(name, name_space, edm_type_kind_t::Entity, basetype, isAbstract, isOpenType), m_HasStream(hasStream)
	{
	}

	/// <summary>
	/// Adds a property name to the key of the entity type
	/// </summary>
	void add_key_property(const ::odata::string_t& property_ref)
	{
		m_key.emplace_back(property_ref);
	}

	/// <summary>
	/// Gets the key, a collection of names of properties in the entity.
	/// </summary>
	const std::vector<::odata::string_t>& key() const
	{
		return m_key;
	}

	std::vector<::odata::string_t> get_key_with_parents() const
	{
		std::vector<::odata::string_t> ret = m_key;

		if (m_base_type && std::dynamic_pointer_cast<edm_entity_type>(m_base_type))
		{
			auto parent_keys = std::dynamic_pointer_cast<edm_entity_type>(m_base_type)->get_key_with_parents();
			if (!parent_keys.empty())
			{
				ret.reserve(ret.size() + parent_keys.size());
				std::copy(parent_keys.begin(), parent_keys.end(), std::back_inserter(ret));
			}
		}

		return ret;
	}

private:
	friend class edm_schema;

	std::vector<::odata::string_t> m_key;
	bool m_HasStream;
};

}}
