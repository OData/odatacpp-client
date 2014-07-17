#pragma once

#include "odata/communication/http_communication.h"
#include "odata/edm/odata_edm.h"
#include "odata/edm/edm_model_reader.h"

namespace odata { namespace edm
{

template<typename _edm_model_Impl>
class edm_model_factory
{
public:
	static std::shared_ptr<_edm_model_Impl> create_reader_instance(concurrency::streams::istream stream)
	{
		return std::make_shared<_edm_model_Impl>(stream);
	}

	static std::shared_ptr<_edm_model_Impl> create_writer_instance(std::shared_ptr<edm_model> schema)
	{
		return std::make_shared<_edm_model_Impl>(schema);
	}
};

}}