/*
 * Copyright (c) Microsoft Corporation. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
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
	edm_operation_import(::utility::string_t name, ::utility::string_t operation_name, ::utility::string_t entity_set_path, bool is_in_service_document, OperationImportKind operation_import_kind) :
		m_name(name), m_operation_name(operation_name), m_entity_set_path(entity_set_path), m_is_in_service_document(is_in_service_document), m_operation_import_kind(operation_import_kind)
	{
		m_operation_type = nullptr;
	}

	const ::utility::string_t get_name() const
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

	::utility::string_t m_name;
	::utility::string_t m_operation_name;
	std::shared_ptr<edm_operation_type> m_operation_type;
	::utility::string_t m_entity_set_path;
	bool m_is_in_service_document;
	OperationImportKind m_operation_import_kind;
};

}}