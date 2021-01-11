//---------------------------------------------------------------------
// <copyright file="edm_operation_import.h" company="Microsoft">
//      Copyright (C) Microsoft Corporation. All rights reserved. See License.txt in the project root for license information.
// </copyright>
//---------------------------------------------------------------------

#pragma once

#include "odata/common/utility.h"
#include "odata/edm/edm_type.h"

namespace odata { namespace edm
{

class edm_entity_container;

/// <summary>
///
/// </summary>
enum OperationImportKind
{
	ActionImport = 0,
	FunctionImport
};

/// <summary>
/// Represents a set of operation imports in a container
/// </summary>
class edm_operation_import
{
public:
	edm_operation_import(::odata::string_t name, ::odata::string_t operation_name, ::odata::string_t entity_set_path, bool is_in_service_document, OperationImportKind operation_import_kind) :
		m_name(std::move(name)), m_operation_name(std::move(operation_name)), m_entity_set_path(std::move(entity_set_path)), m_is_in_service_document(is_in_service_document), m_operation_import_kind(operation_import_kind)
	{
		m_operation_type = nullptr;
	}

	const ::odata::string_t get_name() const
	{
		return m_name;
	}

	const std::shared_ptr<edm_operation_type> get_operation_type() const
	{
		return m_operation_type;
	}

	void set_operation_type(std::shared_ptr<edm_operation_type> operationType)
	{
		m_operation_type = operationType;
	}

private:
	friend class edm_entity_container;

	::odata::string_t m_name;
	::odata::string_t m_operation_name;
	std::shared_ptr<edm_operation_type> m_operation_type;
	::odata::string_t m_entity_set_path;
	bool m_is_in_service_document;
	OperationImportKind m_operation_import_kind;
};

}}
