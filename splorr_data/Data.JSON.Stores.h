#pragma once
#include <functional>
#include "json.hpp"
#include <optional>
namespace data::json::Stores
{
	void SetStoreFile(int, const std::string&, std::optional<unsigned char>);
	template<typename TStore>
	std::function<void()> DoSetStoreFile(const TStore& store, const std::string& filename, std::optional<unsigned char> checksum)
	{
		return [store, filename, checksum]() 
		{
			SetStoreFile((int)store, filename, checksum);
		};
	}
	nlohmann::json& GetStore(int);
	template<typename TStore>
	nlohmann::json& GetStore(const TStore& store)
	{
		return GetStore((int)store);
	}
	void Start();
	void Save(int);
	template<typename TStore>
	void Save(const TStore& store)
	{
		Save((int)store);
	}
	bool IsModded();
}
