#pragma once
#include "json.hpp"
#include "Data.JSON.Store.h"
namespace data::json::Stores
{
	nlohmann::json& GetStore(const data::json::Store&);
	void Start();
	void Save(const data::json::Store&);
	bool IsModded();
}
